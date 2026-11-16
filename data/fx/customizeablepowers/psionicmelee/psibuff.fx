#
FxInfo

	LifeSpan 150


###########################################################
// Condition
// 	On 	Time
// 	Time	0
// 	Event
// 		Type	Local
// 		At	Root
// 		Sound 	PsionicMelee_PsiBuff_Concentration 100 100 .8
// 	End
// End

	Condition
		On Time
		Time 1

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings_In_01.part
			Part :PsioHeadRings_In_02.part
			Part :PsioHeadGlow.part
			Part :EnergyCoreFlash.part
			Lifespan 45
		End
	End

	Condition
		On time
		Time 10

		Event
			At HandL
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :PsioHeadRings_In_01.part
			Part :PsioHeadRings_In_02.part
			Part :PsioHeadGlow.part
			Part :EnergyCoreFlash.part
			Lifespan 45
		End

		Event
			At HandR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :PsioHeadRings_In_01.part
			Part :PsioHeadRings_In_02.part
			Part :PsioHeadGlow.part
			Part :EnergyCoreFlash.part
			Lifespan 45
		End
	End


#############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End
