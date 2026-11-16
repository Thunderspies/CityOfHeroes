#
FxInfo

	Flags InheritGeoScale

	Input
		InpName Hips
	End

	LifeSpan 90


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_SteamSpray 100 100 .8
	End
End

	Condition
		On Time
		Time 8

		Event
			At WepL
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			Part :WaterShot_GlowStreaks.part
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
			LifeSpan 45
		End

		Event
			At WepR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			Part :WaterShot_GlowStreaks.part
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				#PositionOffset 0 4 0
				PyrRotate 15 0 0
			End
			Part :Steam_Bonus.part
			LifeSpan 28
		End

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				#PositionOffset 0 4 0
				PyrRotate 15 0 0
			End
			Part :Steam_Clouds.part
			Part :Steam_Clouds_Highlights.part
			LifeSpan 28
		End

	End

	Condition
		On Time
		Time 30

		Event
			At HandR
			Type Local
			EName SteamAnchor
			BhvrOverride
				PyrRotate -15 -90 30
				Scale 10 10 10
				Alpha 1
			End
			Geom FX_ConeOffsets
			LifeSpan 25
		End

		Event
			At HandR
			Type Create
			EName AnchorA
			Inherit Position Rotation #
			Update Position Rotation #
		End

		Event
			At SteamAnchor
			Type Local
			EName AnchorT
			BhvrOverride
				PositionOffset 0 0 -5
			End
		End

		Event
			At AnchorT
			Type Local
			POther AnchorA
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :Steam_Spray_Volumecone.part
			Part :Steam_Spray_VolumeCone_Additive.part
			LifeSpan 18
		End

		Event
			At HandL
			Type Local
			EName SteamAnchorL
			BhvrOverride
				PyrRotate -5 90 30
				Scale 10 10 10
			End
			Geom FX_ConeOffsets
			LifeSpan 18
		End

		Event
			At HandL
			Type Create
			EName AnchorAL
			Inherit Position Rotation #
			Update Position Rotation #
		End

		Event
			At SteamAnchorL
			Type Local
			EName AnchorTL
			BhvrOverride
				PositionOffset 0 0 -5
			End
		End

		Event
			At AnchorTL
			Type Local
			POther AnchorAL
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :Steam_Spray_Volumecone.part
			Part :Steam_Spray_VolumeCone_Additive.part
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 30

		Event
			At AnchorT
			Type Local
			Part :Steam_Clouds_Target.part
			LifeSpan 30
		End

		Event
			At AnchorTL
			Type Local
			Part :Steam_Clouds_Target.part
			LifeSpan 30
		End

	End

End
