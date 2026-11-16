#
FxInfo

	LifeSpan 30


###########################################################
Condition
	On 	Time
	Time	1
	Event
		Type	Local
		At	Root
		Sound 	WB_Geyser_Hit 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			EName Sound
		#	Sound Sewer_SewerGeiser_Start_01 100 100 0.8
		End

		Event
			At Origin
			Type Start
			Part :Bottom_Foam_Flat.part
			Part :Bottom_Foam_Flat.part
		End

		Event
			At Origin
			Type Start
			EName Geyser
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				#PositionOffset 0.0 -0.5 0
				Scale 1.25 1.25 1.25
			End
			Part :SteamAlpha.part
			Part :ViolentSteamCore.part
			Part :ViolentSteamCore2.part
			Part :ViolentSteamCoreAdd.part
			Part :Top_Splash_Spray.part
			Part :Top_Splash.part
			Part :Splash_Base.part
			Part :Splash_Base02.part
			Part :Bottom_Splash_Secondary.part
			#Sound Sewer_SewerGeiser_Loop 100 100 0.8
		End

	End

End
