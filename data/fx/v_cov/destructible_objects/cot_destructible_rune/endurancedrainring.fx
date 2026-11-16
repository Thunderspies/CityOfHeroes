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
		
		BhvrOverride
			
			Scale			1.2 1.2 1.2
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		LifeSpan	40
	End

	Event
		Type	start 
		At	RadiusFxScalex
		
		BhvrOverride
			
			Scale			.5 .5 .5
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:Endurance01Local.part	
		Part	:Endurance02Local.part	
		
		LifeSpan	30
		
	End
	
	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr

		BhvrOverride
			
			Scale			.5 .5 .5
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		Part	:EnduranceBubblesLocal.part
		
		LifeSpan	70
		
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
		
		BhvrOverride
			
			Scale			1.2 1.2 1.2
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		LifeSpan	40
	End

	Event
		Type	start 
		At	RadiusFxScalex
		
		BhvrOverride
			
			Scale			.5 .5 .5
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:Endurance01Local.part	
		Part	:Endurance02Local.part	
		
		LifeSpan	30
		
	End
	
	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr

		BhvrOverride
			
			Scale			.5 .5 .5
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		Part	:EnduranceBubblesLocal.part
		
		LifeSpan	70
		
	End


End


##################################

Condition
	On	Time
	Time	15

	Event
		EName   RadiusFxScalex
		Type	start
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		
		BhvrOverride
			
			FadeInLength		60
			Scale			1.2 1.2 1.2
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		LifeSpan	40
	End

	Event
		Type	start 
		At	RadiusFxScalex
		
		BhvrOverride
			
			Scale			.5 .5 .5
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part	:Endurance01Local.part	
		Part	:Endurance02Local.part	
		
		LifeSpan	30
		
	End
	
	Event
		Type	start 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr

		BhvrOverride
			
			Scale			.5 .5 .5
			ScaleRate		1.5 1.5 1.5
			EndScale		0.01 0.01 0.01
		
		End

		Part	:EnduranceBubblesLocal.part
		
		LifeSpan	70
		
	End


End

End				