#
FxInfo

	Flags DontSuppress


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_UmbraBeast_Loop 100 100 .25
	End
End



	Condition
		On Time
		Time 0


		Event
			At Head
			Type Local
			ChildFx :CHILD_Dog_Eyes.fx
			EName EyeGlows_Male
		End

		Event
			At JAW
			Type Local
			ChildFx :UmbraBeast_Breath.fx
			EName EyeGlows_Male
		End

		Event
			At Neck
			Type Local
			POther RWing2
			Part :UmbraBeast_Smoke_Core_Head.part
		End

		Event
			At Head
			Type Local
			Part :UmbraBeast_Smoke_Core_Head.part
		End

		Event
			At Neck
			Type Local
			POther T2_L
			Part :UmbraBeast_Smoke_Tendrils.part
		End

		Event
			At Chest
			Type Local
			POther Neck
			Part :UmbraBeast_Smoke_Core.part
		End

		Event
			At Lwing1
			Type Local
			POther Waist
			Part :UmbraBeast_Smoke_Core.part
		End

		Event
			At Waist
			Type Local
			POther LWing1
			Part :UmbraBeast_Smoke_Core.part
		End

		Event
			At T2_R
			Type Local
			EName Tail
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Tail.part
		End

		Event
			At T3_R
			Type Local
			EName Tail
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Tail.part
		End

		Event
			At T1_L
			Type Local
			EName Tail
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Tail_Base.part
		End

		Event
			At Fore_UlegR
			Type Local
			POther Fore_LlegR
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Fore_UlegL
			Type Local
			POther Fore_LlegL
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Hind_UlegR
			Type Local
			POther HInd_LlegR
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Hind_UlegL
			Type Local
			POther Hind_LlegL
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Fore_LlegR
			Type Local
			POther Fore_FootR
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Fore_LlegL
			Type Local
			POther Fore_FootL
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Hind_FootR
			Type Local
			POther Hind_LLegR
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Hind_FootL
			Type Local
			POther Hind_LLegL
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Hind_ToeL
			Type Local
			POther Hind_FootL
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Hind_ToeR
			Type Local
			POther Hind_FootR
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		End

		Event
			At Root
			Type PositOnly
			EName RootFX
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Root_Tendrils.part
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Root_Core.part
		End

		Event
			At Root
			Type PositOnly
			EName RootFX
			Part CustomizeablePowers\DarknessControl\Shade_Core_Umbra.part
		End

	End

End
