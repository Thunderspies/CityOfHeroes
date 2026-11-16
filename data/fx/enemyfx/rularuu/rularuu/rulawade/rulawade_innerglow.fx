#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hair
			Type Local
			EName HeadFX
			BhvrOverride
				PositionOffset 0 -0.5 0.2
			End
			Part :Headglow_Main.part
		End

		Event
			At Head
			Type Local
			EName HeadFX
			BhvrOverride
				PositionOffset 0.0 0.1 0.3
			End
			Part :Headglow_Inner.part
		End

		Event
			At Hair
			Type Local
			EName HeadFX
			BhvrOverride
				PositionOffset 0.1 -0.1 0.05
				PyrRotate -70 -30 0
			End
			#Part :GlowStreak_Small.part
			Part :GlowStreak_Medium.part
		End

		Event
			At Hair
			Type Local
			EName HeadFX
			BhvrOverride
				PositionOffset -0.1 -0.3 0.2
				PyrRotate 45 30 0
			End
			Part :GlowStreak_Small.part
			#Part :GlowStreak_Medium.part
		End

		Event
			At Chest
			Type Local
			EName ChestFX
			BhvrOverride
				PositionOffset 0.25 0.2 0
			End
			Part :ChestGlow_Main.part
		End

		Event
			At Chest
			Type Local
			EName OffsetChestGlowHips
			BhvrOverride
				PositionOffset -0.1 -0.4 0.2
			End
		End

		Event
			At Chest
			Type Local
			EName ChestFX
			POther OffsetChestGlowHips
			BhvrOverride
				PositionOffset 0.35 0.45 -0.2
			End
			Part :ChestGlow_Inner.part
		End

		Event
			At Chest
			Type Local
			EName ChestFX2
			BhvrOverride
				PositionOffset 0.5 0.45 0
			End
			Part :ArmGlow_Shoulder2.part
		End

		Event
			At Chest
			Type Local
			EName ChestFX
			BhvrOverride
				PositionOffset 0.27 0.43 -0.3
				PyrRotate 40 90 0
			End
			Part :GlowStreak_Large.part
		End

		Event
			At Chest
			Type Local
			EName ChestFX
			BhvrOverride
				PositionOffset 0.38 0.3 0.4
				PyrRotate 40 90 0
			End
			Part :GlowStreak_Medium.part
		End

		Event
			At UArmR
			Type Local
			EName ArmFX
			Part :ArmGlow_Shoulder.part
		End

		Event
			At UArmR
			Type Local
			EName ArmFX
			BhvrOverride
				PositionOffset -0.2 0.00 0.1
				PyrRotate -30 90 0
			End
			Part :GlowStreak_Medium.part
		End

		Event
			At UArmL
			Type Local
			EName ArmFX
			POther LArmL
			Part :ArmGlow_Shoulder.part
			Part :ArmGlow_UArm.part
		End

		Event
			At UArmL
			Type Local
			EName ArmFX
			BhvrOverride
				PositionOffset -0.1 0.12 0.1
				PyrRotate 55 0 10
			End
			Part :GlowStreak_Medium.part
		End

		Event
			At LArmR
			Type Local
			EName ArmFX
			BhvrOverride
				PositionOffset 0.25 0.0 0.0
				PyrRotate 0 0 90
			End
			Part :GlowStreak_Large.part
			Part :ArmGlow_LArmR.part
		End

		Event
			At LarmL
			Type Local
			EName FX
			BhvrOverride
				PositionOffset -0.25 0.0 0.0
				PyrRotate 0 0 90
			End
			Part :GlowStreak_Large.part
			Part :ArmGlow_LArmR.part
		End

	End

End
