#############################################################
## MassLevitate.fx
#############################################################

FxInfo
	LifeSpan 60

###########################################################
// Condition
// 	On 	Time
// 	Time	0
// 	Event
// 		Type	Local
// 		At	Root
// 		Sound 	PsionicMelee_MassLevitate 100 100 .8
// 	End
// End

	Condition
		On Time
		Time 1

		Event
			At Head
			Type Local
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings01.part
			Part :PsioHeadRings02.part
			Part :PsioHeadGlow.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Part :Insight_Waves.part
			Lifespan 50
		End
	End

	Condition
		On Time
		Time 18

		Event
			Ename RingAnchor
			At Root
			Type PositOnly
			Part :AOE_GroundRings.part
			Part :AOE_GroundRings_Highlight.part
			Lifespan 45
		End
	End

	Condition
		On Time
		Time 32

		Event
			At RingAnchor
			Type Local
			Bhvr behaviors\CameraShake_Strong30.bhvr
			BhvrOverride
				PositionOffset 0 2 0
			End
			Part :AOE_Rings.part
			Part :AOE_Rings_Highlight.part
			Lifespan 32
		End
	End

	Condition
		On Time
		Time 28

		Event
			At WepL
			Type Local
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings01.part
			Part :PsioHeadRings02.part
			Part :PsioHeadGlow.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Lifespan 50
		End

		Event
			At WepR
			Type Local
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings01.part
			Part :PsioHeadRings02.part
			Part :PsioHeadGlow.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Lifespan 50
		End

		Event
			Ename GroundAnchor
			At Root
			Type Local
		End
	End

End
