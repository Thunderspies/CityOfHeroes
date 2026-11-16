#                                                          
FxInfo

	Input
		InpName Origin
	End

	LifeSpan 90


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			Sound ChumHit1 80 80 0.99
		End

	End

	Condition
		On Time
		Time 0

		Event
			At UArmR
			Type Local
			POther LarmR
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 100
		End

		Event
			At LArmR
			Type Local
			POther HandR
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 100
		End

		Event
			At ULegR
			Type Local
			POther LlegR
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 100
		End

		Event
			At UArmL
			Type Local
			POther LarmL
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			POther HandL
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 100
		End

		Event
			At ULegL
			Type Local
			POther LlegL
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 100
		End

	End

	Condition
		On Time
		Time 0

		Event
			At UArmR
			Type Local
			POther LarmR
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 120
		End

		Event
			At LArmR
			Type Local
			POther HandR
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 120
		End

		Event
			At ULegR
			Type Local
			POther LlegR
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 120
		End

		Event
			At UArmL
			Type Local
			POther LarmL
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 120
		End

		Event
			At LArmL
			Type Local
			POther HandL
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 120
		End

		Event
			At ULegL
			Type Local
			POther LlegL
			Part :V_Mako_WaterDrips.part
			Part :V_Mako_WaterDripsRed.part
			Part :V_Mako_Chumdrops.part
			LifeSpan 120
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type PositOnly
			EName splashhead
			LookAt T_Head
		End

		Event
			At splashhead
			Type Local
			PMagnet root
			BhvrOverride
				PositionOffset 0 0 0.4
				PyrRotate -90 0 0
			End
			Part :V_Mako_ChumSpittleSplash.part
			Part :V_Mako_ChumSpittleSplashRed.part
			LifeSpan 45
		End

		Event
			At UArmR
			Type PositOnly
			EName splasharmR
			LookAt T_Head
		End

		Event
			At splasharmR
			Type Local
			PMagnet root
			BhvrOverride
				PositionOffset 0 0 .4
				PyrRotate -90 0 0
			End
			Part :V_Mako_ChumSpittleSplash.part
			Part :V_Mako_ChumSpittleSplashRed.part
			LifeSpan 45
		End

		Event
			At UArmL
			Type PositOnly
			EName splasharmL
			LookAt T_Head
		End

		Event
			At splasharmL
			Type Local
			PMagnet root
			BhvrOverride
				PositionOffset 0 0 .4
				PyrRotate -90 0 0
			End
			Part :V_Mako_ChumSpittleSplash.part
			Part :V_Mako_ChumSpittleSplashRed.part
			LifeSpan 45
		End

		Event
			At LLegR
			Type PositOnly
			EName splashlegR
			LookAt T_Head
		End

		Event
			At splashlegR
			Type Local
			PMagnet root
			BhvrOverride
				PositionOffset 0 0 .4
				PyrRotate -90 0 0
			End
			Part :V_Mako_ChumSpittleSplash.part
			Part :V_Mako_ChumSpittleSplashRed.part
			LifeSpan 45
		End

		Event
			At LLegL
			Type PositOnly
			EName splashlegL
			LookAt T_Head
		End

		Event
			At splashlegL
			Type Local
			PMagnet root
			BhvrOverride
				PositionOffset 0 0 .4
				PyrRotate -90 0 0
			End
			Part :V_Mako_ChumSpittleSplash.part
			Part :V_Mako_ChumSpittleSplashRed.part
			LifeSpan 45
		End

	End

End
