#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			Type Local
			At Root
			Sound FW_PosessedConduitFloat_Loop 50 50 0.5
		End

	End

	Condition
		On Time
		Time 0

		Event
			Type Local
			At Hair
			POther Neck
			Part :BodyGlow_Medium2.part
		End

		Event
			Type Local
			At Chest
			POther Chest
			Part Scripted\Praetorian\PossessedCondiut_Float\BodyGlow_Large3.part
			Part :BodyGlow_Large1.part
			Part :BodyGlow_Large2.part
			Part :BodyGlow_Rays1.part
			Part :BodyGlow_Rays2.part
		End

		Event
			Type Local
			At LArmL
			POther UArmL
			Part :BodyGlow_Medium2.part
		End

		Event
			Type Local
			At LArmR
			POther UArmR
			Part :BodyGlow_Medium2.part
		End

		Event
			Type Local
			At HandL
			POther LArmL
			Part :BodyGlow_Medium1.part
			Part :BodyGlow_Medium2.part
			Part Scripted\Praetorian\PossessedCondiut_Float\BodyGlow_Medium1b.part
		End

		Event
			Type Local
			At ULegR
			POther ULegL
			Part :BodyGlow_Medium1.part
			Part :BodyGlow_Medium2.part
			Part Scripted\Praetorian\PossessedCondiut_Float\BodyGlow_Medium1b.part
		End

		Event
			Type Local
			At Hips
			POther Hips
			Part :BodyGlow_Large1.part
			Part :BodyGlow_Large2.part
		End

		Event
			Type Local
			At LLegL
			POther ULegL
			Part :BodyGlow_Medium1.part
			Part :BodyGlow_Medium2.part
			Part Scripted\Praetorian\PossessedCondiut_Float\BodyGlow_Medium1b.part
		End

		Event
			Type Local
			At LLegR
			POther ULegR
			Part :BodyGlow_Medium1.part
			Part :BodyGlow_Medium2.part
			Part Scripted\Praetorian\PossessedCondiut_Float\BodyGlow_Medium1b.part
		End

		Event
			Type Local
			At FootL
			POther LLegL
			Part :BodyGlow_Medium1.part
			Part :BodyGlow_Medium2.part
			Part Scripted\Praetorian\PossessedCondiut_Float\BodyGlow_Medium1b.part
		End

		Event
			Type Local
			At FootR
			POther LLegR
			Part :BodyGlow_Medium1.part
			Part :BodyGlow_Medium2.part
			Part Scripted\Praetorian\PossessedCondiut_Float\BodyGlow_Medium1b.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			EName GodRayOffset1
			Type Local
			At Root
			BhvrOverride
				PositionOffset 0 128 0
			End
		End

		Event
			Type PositOnly
			At Hips
			POther GodRayOffset1
			Part :Cage_GodRay1.part
		End

	End

End
