#############################################################
## TimesJuncture_Attack.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_TimesJuncture_Continuing_Loop 100 100 .65
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_TimesJuncture_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	15


	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Swirl_Quick_Hands.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	17

	Event

		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Swirl_Quick_Hands.part
		Lifespan	5
	End

End

Condition
	On	Time
	Time	21

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Swirl_Quick_Hands_Core.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	23

	Event

		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Swirl_Quick_Hands_Core.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	38

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Jaunt_Flash_Hands.part
		Part	CustomizeablePowers\TimeManipulation\Jaunt_Glow_Hands.part
		Lifespan	5
	End

	Event
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Jaunt_Flash_Hands.part
		Part	CustomizeablePowers\TimeManipulation\Jaunt_Glow_Hands.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	40

	Event

		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Streaks_Hands.part
		Part	CustomizeablePowers\TimeManipulation\Streaks_Hands_Detached.part
		Lifespan	5
	End

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Streaks_Hands.part
		Part	CustomizeablePowers\TimeManipulation\Streaks_Hands_Detached.part
		Lifespan	5
	End


End

Condition
	On	Time
	Time	42

	Event

		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Rings_Hands.part
		Part	CustomizeablePowers\TimeManipulation\Spiral_hands.part
		Lifespan	5
	End

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\TimeManipulation\Rings_Hands.part
		Part	CustomizeablePowers\TimeManipulation\Spiral_hands.part
		Lifespan	5
	End



End

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Root
		Part	CustomizeablePowers\TimeManipulation\Spiral_Ground_Perimeter.part
#		Part	CustomizeablePowers\TimeManipulation\Spiral_Ground_Shrinking.part
#		Part	CustomizeablePowers\TimeManipulation\Spiral_Ground_Shrinking_Additive.part
		Part	CustomizeablePowers\TimeManipulation\Particles_Slow_Big.part
		PMagnet	Chest
	End
End

#############################################################

End