#############################################################
## TemporalSelection_Attack.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time	18
	Event
		Type	Local
		At	Root
		Sound 	TM_TemporalSelection_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	17

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Swirl_Quick_Hands.part
		Lifespan	5
	End

End

Condition
	On	Time
	Time	23

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Swirl_Quick_Hands_Core.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	40

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Jaunt_Flash_Hands.part
		Part	:Jaunt_Glow_Hands.part
		Part	:Streaks_Hands.part
		Part	:Streaks_Hands_Detached.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	42

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Hands.part
		Part	:Spiral_hands.part
		Lifespan	5
	End
End

#############################################################

End