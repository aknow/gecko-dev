/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is Mozilla MathML Project.
 * 
 * The Initial Developer of the Original Code is The University Of 
 * Queensland.  Portions created by The University Of Queensland are
 * Copyright (C) 1999 The University Of Queensland.  All Rights Reserved.
 * 
 * Contributor(s): 
 *   Roger B. Sidje <rbs@maths.uq.edu.au>
 *   David J. Fiddes <D.J.Fiddes@hw.ac.uk>
 *   Shyjan Mahamud <mahamud@cs.cmu.edu> (added TeX rendering rules)
 */

#include "nsCOMPtr.h"
#include "nsHTMLParts.h"
#include "nsIHTMLContent.h"
#include "nsFrame.h"
#include "nsLineLayout.h"
#include "nsHTMLIIDs.h"
#include "nsIPresContext.h"
#include "nsHTMLAtoms.h"
#include "nsUnitConversion.h"
#include "nsIStyleContext.h"
#include "nsStyleConsts.h"
#include "nsINameSpaceManager.h"
#include "nsIRenderingContext.h"
#include "nsIFontMetrics.h"
#include "nsStyleUtil.h"

#include "nsMathMLmsupFrame.h"

//
// <msup> -- attach a superscript to a base - implementation
//

nsresult
NS_NewMathMLmsupFrame(nsIPresShell* aPresShell, nsIFrame** aNewFrame)
{
  NS_PRECONDITION(aNewFrame, "null OUT ptr");
  if (nsnull == aNewFrame) {
    return NS_ERROR_NULL_POINTER;
  }
  nsMathMLmsupFrame* it = new (aPresShell) nsMathMLmsupFrame;
  if (nsnull == it) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
  *aNewFrame = it;
  return NS_OK;
}

nsMathMLmsupFrame::nsMathMLmsupFrame()
{
}

nsMathMLmsupFrame::~nsMathMLmsupFrame()
{
}

NS_IMETHODIMP
nsMathMLmsupFrame::Init(nsIPresContext*  aPresContext,
			nsIContent*      aContent,
			nsIFrame*        aParent,
			nsIStyleContext* aContext,
			nsIFrame*        aPrevInFlow)
{
  nsresult rv = nsMathMLContainerFrame::Init
    (aPresContext, aContent, aParent, aContext, aPrevInFlow);

  mSupScriptShiftFactor = 0.0f;
  mScriptSpace = NSFloatPointsToTwips(0.5f); // 0.5pt as in plain TeX

  // check for superscriptshift attribute in ex units
  nsAutoString value;
  mUserSetFlag = PR_FALSE;
  if (NS_CONTENT_ATTR_HAS_VALUE == mContent->GetAttribute
      (kNameSpaceID_None, nsMathMLAtoms::superscriptshift_, value)) {
    PRInt32 aErrorCode;
    float aUserValue = value.ToFloat(&aErrorCode);
    if (NS_SUCCEEDED(aErrorCode)) {
      mUserSetFlag = PR_TRUE;
      mSupScriptShiftFactor = aUserValue;
    }
  }

  mPresentationData.flags |= NS_MATHML_SHOW_BOUNDING_METRICS;
  return rv;
}

NS_IMETHODIMP
nsMathMLmsupFrame::Place(nsIPresContext*      aPresContext,
                         nsIRenderingContext& aRenderingContext,
                         PRBool               aPlaceOrigin,
                         nsHTMLReflowMetrics& aDesiredSize)
{
  nsresult rv = NS_OK;

  ////////////////////////////////////
  // Get the children's desired sizes

  PRInt32 count = 0;
  nsRect aRect;
  nsHTMLReflowMetrics baseSize (nsnull);
  nsHTMLReflowMetrics supScriptSize (nsnull);
  nsIFrame* baseFrame;
  nsIFrame* supScriptFrame;
  // parameter u in Rule 18a, Appendix G of the TeXbook
  nscoord minSupScriptShift = 0;   

  nsBoundingMetrics bmBase, bmSupScript;

  nsIFrame* aChildFrame = mFrames.FirstChild(); 
  while (nsnull != aChildFrame) 
  {
    if (!IsOnlyWhitespace(aChildFrame)) {
      if (0 == count) {
	// base 
	baseFrame = aChildFrame;
        GetReflowAndBoundingMetricsFor(baseFrame, baseSize, bmBase);
      }
      else if (1 == count) {
	// superscript
	supScriptFrame = aChildFrame;
        GetReflowAndBoundingMetricsFor(supScriptFrame, supScriptSize, bmSupScript);
	// get the supdrop from the supscript font
	nscoord aSupDrop;
	GetSupDropFromChild (aPresContext, supScriptFrame, aSupDrop);
	// parameter u, Rule 18a, App. G, TeXbook
	minSupScriptShift = bmBase.ascent - aSupDrop;
      }
      else {
	NS_ASSERTION((count < 2),"nsMathMLmsupFrame : invalid markup");
      }
      count++;
    }
    rv = aChildFrame->GetNextSibling(&aChildFrame);
    NS_ASSERTION(NS_SUCCEEDED(rv),"failed to get next child");
  }

  //////////////////
  // Place Children 
  
  // get min supscript shift limit from x-height
  // = d(x) + 1/4 * sigma_5, Rule 18c, App. G, TeXbook
  nscoord xHeight = 0;
  nsCOMPtr<nsIFontMetrics> fm;

//  const nsStyleFont* aFont =
//    (const nsStyleFont*) mStyleContext->GetStyleData (eStyleStruct_Font);

  const nsStyleFont *aFont;
  baseFrame->GetStyleData(eStyleStruct_Font, (const nsStyleStruct *&)aFont);

  aPresContext->GetMetricsFor (aFont->mFont, getter_AddRefs(fm));
  fm->GetXHeight (xHeight);
  nscoord minShiftFromXHeight = (nscoord) 
    (bmSupScript.descent + (1.0f/4.0f) * xHeight);

  // aSupScriptShift{1,2,3}
  // = minimum amount to shift the supscript up
  // = sup{1,2,3} in TeX
  // aSupScriptShift1 = superscriptshift attribute * x-height
  // Note that there are THREE values for supscript shifts depending
  // on the current style
  nscoord aSupScriptShift1, aSupScriptShift2, aSupScriptShift3;
  // Set aSupScriptShift{1,2,3} default from font
  GetSupScriptShifts (fm, aSupScriptShift1, aSupScriptShift2, aSupScriptShift3);

  if (mUserSetFlag) {
    // the user has set the supscriptshift attribute
    float aFactor2 = ((float) aSupScriptShift2) / aSupScriptShift1;
    float aFactor3 = ((float) aSupScriptShift3) / aSupScriptShift1;
    aSupScriptShift1 = NSToCoordRound(mSupScriptShiftFactor * xHeight);
//XXX shouldn't this be 
//    aSupScriptShift1 = 
//      PR_MAX(aSupScriptShift1, NSToCoordRound(mSupScriptShiftFactor * xHeight));
    aSupScriptShift2 = NSToCoordRound(aFactor2 * aSupScriptShift1);
    aSupScriptShift3 = NSToCoordRound(aFactor3 * aSupScriptShift1);
  }

  // get sup script shift depending on current script level and display style
  // Rule 18c, App. G, TeXbook
  nscoord aSupScriptShift;
  if ( mPresentationData.scriptLevel == 0 && 
       NS_MATHML_IS_DISPLAYSTYLE(mPresentationData.flags) &&
      !NS_MATHML_IS_COMPRESSED(mPresentationData.flags)) {
    // Style D in TeXbook
    aSupScriptShift = aSupScriptShift1;
  }
  else if (NS_MATHML_IS_COMPRESSED(mPresentationData.flags)) {
    // Style C' in TeXbook = D',T',S',SS'
    aSupScriptShift = aSupScriptShift3;
  }
  else {
    // everything else = T,S,SS
    aSupScriptShift = aSupScriptShift2;
  }
  
  // get actual supscriptshift to be used
  // Rule 18c, App. G, TeXbook
  nscoord actualSupScriptShift = 
    PR_MAX(minSupScriptShift,PR_MAX(aSupScriptShift,minShiftFromXHeight));

  mBoundingMetrics.ascent =
    PR_MAX(bmBase.ascent, (bmSupScript.ascent + actualSupScriptShift));
  mBoundingMetrics.descent =
    PR_MAX(bmBase.descent, (bmSupScript.descent - actualSupScriptShift));
  // add mScriptSpace between base and supscript
  mBoundingMetrics.width = bmBase.width + mScriptSpace + bmSupScript.width;

  // to be simplified ...
  nscoord dyBase = mBoundingMetrics.ascent - bmBase.ascent;
  nscoord dySupScript = mBoundingMetrics.ascent - bmSupScript.ascent - actualSupScriptShift;

  nscoord baseTop = mBoundingMetrics.ascent - dyBase - bmBase.ascent + baseSize.ascent;
  nscoord supScriptTop = mBoundingMetrics.ascent - dySupScript - bmSupScript.ascent + supScriptSize.ascent;

  aDesiredSize.ascent = PR_MAX(baseTop, supScriptTop);
  aDesiredSize.descent = PR_MAX(baseSize.height-baseTop, supScriptSize.height-supScriptTop);
  aDesiredSize.height = aDesiredSize.ascent + aDesiredSize.descent;

  aDesiredSize.width = mBoundingMetrics.width;

  mReference.x = 0;
  mReference.y = aDesiredSize.ascent - mBoundingMetrics.ascent;
  mBoundingMetrics.leftBearing = bmBase.leftBearing;
  mBoundingMetrics.rightBearing = baseSize.width + mScriptSpace + bmSupScript.rightBearing;

  if (aPlaceOrigin) {
    nscoord dx, dy;
    // now place the base ...
    dx = 0; dy = aDesiredSize.ascent - baseTop;
    FinishReflowChild (baseFrame, aPresContext, baseSize, dx, dy, 0);
    // ... and supscript
    dx = baseSize.width + mScriptSpace; 
    dy = aDesiredSize.ascent - supScriptTop;
    FinishReflowChild (supScriptFrame, aPresContext, supScriptSize, dx, dy, 0);
  }

  return NS_OK;
}
