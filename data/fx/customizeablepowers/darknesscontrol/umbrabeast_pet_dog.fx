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
		Sound 	DC_UmbraBeast_Hit_Loop 100 100 .5
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
			Flags OneAtATime
		End

		Event
			At Hips
			Type PositOnly
			POther Neck
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core.part
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Tendrils.part
		End

		#Event
			#At Fore_ToeL
			#Type PositOnly
			#EName FootL
			#Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		#End

		#Event
			#At Fore_ToeR
			#Type PositOnly
			#EName FootR
			#Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		#End

		#Event
			#At Hind_ToeL
			#Type PositOnly
			#EName HindFootL
			#Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		#End

		#Event
			#At Hind_ToeR
			#Type PositOnly
			#EName HindFootR
			#Part CustomizeablePowers\DarknessControl\UmbraBeast_Smoke_Core_Small.part
		#End

		Event
			At Root
			Type PositOnly
			EName RootFX
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Root_Tendrils.part
			Part CustomizeablePowers\DarknessControl\UmbraBeast_Root_Core.part
		End

	End

End
