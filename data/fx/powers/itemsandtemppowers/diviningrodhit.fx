#########################################################
##	ChillingTouchHands
FxInfo

LifeSpan		25

##################################

Condition
	On	Time
	Time	0
	
	Event	
		Type	Local
		At	Root
		Bhvr	:ScaleIn.bhvr
		Geom	ForceRepulsion01
		Sound DiviningRodHit 80 80 .9
		LifeSpan	10
						
	End
	
	Event	
		Type	Local
		At	Root
		Bhvr	:ScaleIn2.bhvr
		Geom	ForceRepulsion02
		LifeSpan	10
						
	End
	
	Event	
		Type	Local
		At	Root
		Bhvr	:ScaleIn3.bhvr
		Geom	ForceRepulsion03
		LifeSpan	10
						
	End
	
	Event	
		Type	Local
		At	Root
		Bhvr	:ScaleIn4.bhvr
		Geom	ForceRepulsion04
		LifeSpan	10
						
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	Root
		Bhvr	:BeamHitOffset.bhvr
		Part	:Eclipse.part
		Part	:EclipseDark.part
		Part	:EclipseThinDark.part
		
		LifeSpan		4
				
	End

End

Condition
	On	Time
	Time	4

	Event	
		Ename   LeftHandMist
		Type	Local
		At	Root
		Bhvr	:BeamHitOffset.bhvr
		Part	:LightBeams.part
		Part	:LightBeamsDown.part
		
		LifeSpan		10		
	End

End
	
End			