#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_WellDrain_Loop 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName PowerSuck
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 3.5 0
				TintGeom 1
				Alpha 80
				FadeInLength 60
				FadeOutLength 60
				#StartColor 255 255 255
			End
			Part ENEMYFX\Praetorian\MoM\Well_PowerDots.part
			Part ENEMYFX\Praetorian\MoM\Well_BottomGlow.part
			Geom Tintable_TaperedSkirtSwirls02
		End

	End

End
