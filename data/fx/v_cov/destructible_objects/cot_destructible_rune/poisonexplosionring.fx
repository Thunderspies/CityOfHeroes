#########################################################
##	chill touch hit
FxInfo

LifeSpan	80

##################################

Condition

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
		Part	:GreenPoisonHitLocal.part	
		Part	:GreenPoisonHitLocal.part
		Part	:GreenPoisonHitLocalDark.part
		
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

##################################

Condition
	On	Time
	Time	5

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
		Part	:GreenPoisonHitLocal.part	
		Part	:GreenPoisonHitLocal.part
		Part	:GreenPoisonHitLocalDark.part
		
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

##################################

Condition
	On	Time
	Time	17

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
		Part	:GreenPoisonHitLocal.part	
		Part	:GreenPoisonHitLocal.part
		Part	:GreenPoisonHitLocalDark.part
		
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