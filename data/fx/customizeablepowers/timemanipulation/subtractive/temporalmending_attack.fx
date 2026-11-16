#############################################################
## Android_Plasma_ChargedBlast.fx
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
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_TemporalMending_Attack_01 100 100 .8
	End
End


Condition
	On	Time
	Time	15


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
	Time	17

	Event

		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Swirl_Quick_Hands.part
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
		Part	:Swirl_Quick_Hands_Core.part
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
		Part	:Swirl_Quick_Hands_Core.part
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
		Part	:Jaunt_Flash_Hands.part
		Part	:Jaunt_Glow_Hands.part
		Part	:Streaks_Hands.part
		Part	:Streaks_Hands_Detached.part
		Lifespan	5
	End

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
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Hands.part
		Part	:Spiral_hands.part
		Lifespan	5
	End

	Event

		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Hands.part
		Part	:Spiral_hands.part
		Lifespan	5
	End



End

Condition
	On	Time
	Time	44

	Event
		Type	PositOnly
		At	Hips
		Part	:Swirls_Explode_Fast_Flat.part
		Lifespan 15
	End

	Event
		Type	PositOnly
		At	Root
		Part	:Spiral_Explode_Fast_Flat.part
		Part	:Spiral_Explode_Fast_Additive_Flat.part
		Part	:Streaks_Explode.part
		Part	:Streaks_Explode_Detached.part
		Part	:Particles_Explode.part

		Lifespan 15
	End
End

#############################################################

End