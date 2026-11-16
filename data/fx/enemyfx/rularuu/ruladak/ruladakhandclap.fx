#########################################################
##	Fireweapon
########################################################
FxInfo


LifeSpan	70
######################################################################


Condition
	On 	Time
	Time 	25

	Event
		EName 	Mallet1
		Type	local 
		At	wepL
		Geom	SuperStrengthBubble
		bhvr	behaviors/ScaleBubble.bhvr
		
	End

	Event
		EName 	Mallet2
		Type	local 
		At	wepL
		bhvr	behaviors/PivotOffsetWepL.bhvr
		Part	:RuladakHandClapHitEmber.part
		Part	:RuladakHandClapHitStar.part
		Part	:RuladakHandClapHitRing.part
		Part	:RuladakHandClapHitRing2.part
		Part	:RuladakHandClapHitGlows2.part
	
		Sound PunchHit2 100.0 100.0 .8
	End

	Event
		EName 	Mallet2
		Type	local 
		At	wepL
		bhvr	behaviors/PivotOffsetWepL.bhvr
		
		Part	:BubbleHandDots.part
		
	End

End


Condition
	On 	Time
	Time 	37

	Event
		EName	Mallet1
		Type	Destroy
		
	End
	
	
End


End
##################################

