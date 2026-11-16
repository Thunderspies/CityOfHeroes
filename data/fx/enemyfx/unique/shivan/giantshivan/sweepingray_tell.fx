#############################################################
## SweepingRay_Attack.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	T_Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Windup_Ring_Shrinking.part
		Lifespan	12
	End

End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	T_Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Windup_Ring_Shrinking_Core.part
		Lifespan	15
	End

End

Condition
	On	Time
	Time	35

	Event
		Type	StartPositOnly
		At	T_Head
		Part	:Windup_Flash_Core.part
		Part	:Windup_Flash.part
		Lifespan	10
	End

End
Condition
	On	Time
	Time	90

	Event
		Type	StartPositOnly
		At	T_Head
		Part	:Windup_Flash_Quick.part
		Lifespan	15
	End

End

#############################################################

End