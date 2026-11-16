#                                                          
FxInfo

	Flags InheritAlpha DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			Anim FX_wings_Celestial_Open
			At Chest
			Type Local
			EName WingSkeleton
			Bhvr behaviors/WingScale_Angel_Male.bhvr
			BhvrOverride
				PositionOffset 0 -0.5 -0.8
				Scale 0.55 0.55 0.55
				Alpha 1
				StartColor 0 0 255
			End
		End

		Event
			AnimPiv HandL
			At WingSkeleton
			Type Local
			EName HandLAnchor
		End

		Event
			AnimPiv HandR
			At WingSkeleton
			Type Local
			EName HandRAnchor
		End

		Event
			AnimPiv FootR
			At WingSkeleton
			Type Local
			EName FootRAnchor
		End

		Event
			AnimPiv FootL
			At WingSkeleton
			Type Local
			EName FootLAnchor
		End

		Event
			AnimPiv ToeR
			At WingSkeleton
			Type Local
			EName ToeRAnchor
		End

		Event
			AnimPiv ToeL
			At WingSkeleton
			Type Local
			EName ToeLAnchor
		End

		Event
			AnimPiv RingL
			At WingSkeleton
			Type Local
			EName WingJointL
			LookAt HandLAnchor
			POther HandLAnchor
			Part :Celestial_Tendril_Consistent.part
			Part :Celestial_Tendril.part
		End

		Event
			AnimPiv RingR
			At WingSkeleton
			Type Local
			EName WingJointR
			LookAt HandRAnchor
			POther HandRAnchor
			Part :Celestial_Tendril_Consistent.part
			Part :Celestial_Tendril.part
		End

		Event
			AnimPiv Fore_FootL
			At WingSkeleton
			Type Local
			EName WingTopL
			LookAt WingJointL
			POther WingJointL
			Part :Celestial_Tendril.part
			Part :Celestial_Tendril_Core.part
		End

		Event
			AnimPiv Fore_FootR
			At WingSkeleton
			Type Local
			EName WingTopR
			LookAt WingJointR
			POther WingJointR
			Part :Celestial_Tendril.part
			Part :Celestial_Tendril_Core.part
		End

		Event
			AnimPiv Fore_LLegL
			At WingSkeleton
			Type Local
			EName WingMiddleL
			POther FootLAnchor
			Part :Celestial_Tendril.part
			Part :Celestial_Tendril_Spread.part
		End

		Event
			AnimPiv Fore_LLegR
			At WingSkeleton
			Type Local
			EName WingMiddleR
			POther FootRAnchor
			Part :Celestial_Tendril.part
			Part :Celestial_Tendril_Spread.part
		End

		Event
			AnimPiv Fore_ToeL
			At WingSkeleton
			Type Local
			EName WingBottomL
			POther ToeLAnchor
			Part :Celestial_Tendril_Consistent.part
			Part :Celestial_Tendril_Spread2.part
		End

		Event
			AnimPiv Fore_ToeR
			At WingSkeleton
			Type Local
			EName WingBottomR
			POther ToeRAnchor
			Part :Celestial_Tendril_Consistent.part
			Part :Celestial_Tendril_Spread2.part
		End

	End

	Condition
		On TriggerBits
		TriggerBits FLY
		DoMany 1

		Event
			At Origin
			Type Local
			ChildFx :FX_Wings_Celestial_Armor_CHILD.fx
			EName WingTrailL
			Flags OneAtATime
			Until STEPLEFT
		End

	End

End
