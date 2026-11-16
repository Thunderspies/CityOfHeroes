#########################################################
##	template

FxInfo

LifeSpan 1000

Condition

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	powers/firecontrol/Inferno.bhvr
		
	End


	Event
		EName 	Prime1
		Type	start 
		At	RadiusFxScale
		bhvr    weapons/bow/trickarrow/Infernobubble.bhvr
		Part	:PoisonYellow2.part
		Part	:PoisonGlows.part
		
		
	End		

End

End

		