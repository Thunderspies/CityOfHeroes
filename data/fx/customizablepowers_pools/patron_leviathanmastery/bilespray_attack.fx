#                                                          
FxInfo

	LifeSpan 140


###########################################################

	Condition
		On Time
		Time 8

		Event
			At Origin
			Type Local
			Sound ChumSpew1 95 95 0.9
		End

	End

	Condition
		On Time
		Time 32

		Event
			At Head
			Type PositOnly
			EName header
			LookAt T_head
			LifeSpan 50
		End

		Event
			At header
			Type Local
			BhvrOverride
				PositionOffset 0 0 -2
			End
			Part :V_Mako_ChumWater.part
			Part :V_Mako_ChumWaterRed.part
			Part CustomizablePowers_Pools\Patron_LeviathanMastery\V_Mako_ChumChunks2.part
			Part :V_Mako_ChumChunks.part
		End

		Event
			At header
			Type Local
			Part :V_Mako_ChumSpittle.part
			Part :V_Mako_ChumSpittleRed.part
		End

		Event
			At header
			Type Local
			EName PushForce
			BhvrOverride
				PositionOffset 0 0 7.5
				PyrRotate 15 0 0
				physForceType Forward
				physForceRadius 10
				physForcePower 150
				physForcePowerJitter 100
			End
			LifeSpan 75
		End

		Event
			At header
			Type Local
			EName PushForce2
			BhvrOverride
				PositionOffset 0 0 25
				PyrRotate 15 0 0
				physForceType Forward
				physForceRadius 20
				physForcePower 75
				physForcePowerJitter 50
			End
			LifeSpan 75
		End

	End

	Condition
		On Time
		Time 0

		Event
			At JAW
			Type Local
			EName jawfront
			BhvrOverride
				PositionOffset 0 -.2 .3
			End
			#Geom testing_Target
		End

		Event
			At JAW
			Type Local
			EName jawsideL
			BhvrOverride
				PositionOffset -.18 -.1 .1
			End
			#Geom testing_Target
		End

		Event
			At JAW
			Type Local
			EName jawsideR
			BhvrOverride
				PositionOffset .18 -.1 .1
			End
			#Geom testing_Target
		End

		Event
			At jawfront
			Type Local
			POther jawsideL
			Part :V_Mako_ChumDrool.part
			Part :V_Mako_ChumDroolRed.part
			Part :V_Mako_ChumDrool.part
			Part :V_Mako_ChumDroolRed.part
			LifeSpan 75
		End

		Event
			At jawfront
			Type Local
			POther jawsideR
			Part :V_Mako_ChumDrool.part
			Part :V_Mako_ChumDroolRed.part
			Part :V_Mako_ChumDrool.part
			Part :V_Mako_ChumDroolRed.part
			LifeSpan 75
		End

	End

	Condition
		On Time
		Time 20

		Event
			At jawfront
			Type Local
			POther jawsideL
			Part :V_Mako_ChumDrool.part
			Part :V_Mako_ChumDroolRed.part
			LifeSpan 100
		End

		Event
			At jawfront
			Type Local
			POther jawsideR
			Part :V_Mako_ChumDrool.part
			Part :V_Mako_ChumDroolRed.part
			LifeSpan 100
		End

	End

End
