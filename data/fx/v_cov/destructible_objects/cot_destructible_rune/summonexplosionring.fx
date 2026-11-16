#########################################################
##	chill touch hit
FxInfo

LifeSpan	80

##################################

Condition

	Event
		Type	start
		At	root

		BhvrOverride	
			PositionOffset	0 -3 0
		End

		Part	:SummonThinLightRaysUp.part
		LifeSpan	15
	End

	Event
		Type	start
		At	root
		Part	:SummonLightRaysUp.part
		LifeSpan	15
	End

	Event
		EName   RadiusFxScalex
		Type	start
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	40
	End

	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:SummonHitLocal.part	
		Part	:SummonHitLocal.part
		Part	:summonHitLocalDark.part
		
		LifeSpan	30
		
	End
	
	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:PoisonHitBlubblesLocal.part
		Part4	:PoisonHitBlubblesLocalWhite.part
		
		LifeSpan	50
		
	End


End

###########################################################

Condition
	On	Time
	Time	10
	Event
		EName   RadiusFxScalex
		Type	start
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	40
	End

	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:SummonHitLocal.part	
		Part	:SummonHitLocal.part
		Part	:summonHitLocalDark.part
		
		LifeSpan	30
		
	End
	
	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:PoisonHitBlubblesLocal.part
		Part4	:PoisonHitBlubblesLocalWhite.part
		
		LifeSpan	50
		
	End


End

######################################################################

Condition
	On	Time
	Time	15

	Event
		EName   RadiusFxScalex
		Type	start
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	40
	End

	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:SummonHitLocal.part	
		Part	:SummonHitLocal.part
		Part	:summonHitLocalDark.part
		
		LifeSpan	30
		
	End
	
	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:PoisonHitBlubblesLocal.part
		Part4	:PoisonHitBlubblesLocalWhite.part
		
		LifeSpan	50
		
	End


End

End				