#
FxInfo

	Flags DontSuppress

	LifeSpan 150


###########################################################

##	Condition
##		On Time
##		Time 25
##
##		Event
##			At Root
##			Type Local
##			BhvrOverride
##				PositionOffset 0 2.75 3.5
##				PyrRotate -70 0 0
##			End
##			Part :Howl_Wind_Streak.part
##			Part :Howl_Wind_Vertical.part
##			LifeSpan 40
##		End
##
##		Event
##			At Head
##			Type Local
##			EName LocknarDummyHookup
##			BhvrOverride
##				PositionOffset 0 -1 0.8
##			End
##		End
##
##		Event
##			At Root
##			Type Local
##			BhvrOverride
##				PyrRotate -70 0 0
##				PositionOffset 0.0 2.75 3.5
##			End
##			Part CustomizeablePowers\BeastMastery\Howl_Rings_Vertical.part
##			LifeSpan 20
##		End
##
##	End

	Condition
		On Time
		Time 35

		Event
			At Root
			Type positOnly
			Ename RingsOut
		End

		Event
			At RingsOut
			Type Local
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part :AOE_CoreRingsOut.part
			Part :AOE_RingsOut.part
			Part :Roar_Ring.part
			Part :Roar_Wind_Shockwave2.part
			Part :Roar_Wind_Shockwave_Core2.part
			LifeSpan 30
		End

		Event
			At RingsOut
			Type Local
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part ENEMYFX\Praetorian\Talons\AOE_CoreRingsOut.part
			Part ENEMYFX\Praetorian\Talons\AOE_RingsOut.part
			LifeSpan 28
		End

	End

End
