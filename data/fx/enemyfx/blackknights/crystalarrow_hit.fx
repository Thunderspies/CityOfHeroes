#
FxInfo

	LifeSpan 75

	ClampMinScale 0.875 0.875 0.875
	ClampMaxScale 1.125 1.125 1.125

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BlackKnights_CrystalArrow_Hit 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part :Hit.part
			Part :Hit1.part
			Part :Hit2.part
			Part :HitRaysSmall.part
			Part :ColdSmoke_Hit01.part
		End

		Event
			At Chest
			Type Local
			Part :HitStar01Sml.part
			Part :HitStar02Sml.part
			Part :HitStar03Sml.part
			Part :HitGlow01Sml.part
			Part :HitRays01Sml.part
			#Sound pp13 100.0 100.0 0.7
		End

		Event
			At Head
			Type PositOnly
			EName UArmLFX
			POther Neck
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			At Chest
			Type PositOnly
			EName UArmLFX
			POther Hips
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			At UArmL
			Type PositOnly
			EName UArmLFX
			POther LarmL
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			At UArmR
			Type PositOnly
			EName UArmRFX
			POther LArmR
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			At LArmL
			Type PositOnly
			EName LArmLFX
			POther HandL
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			At LArmR
			Type PositOnly
			EName LArmRFX
			POther HandR
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			EName	ArrowHitHookup
			Type	create
			Inherit	Position
			Update	Position
			At	CHEST
			LookAt	Target
		End

		Event
			EName	ArrowHit
			Type	create
			Inherit	Position rotation
			Update	Position
			At	ArrowHitHookup
		End

		Event
			Ename	Arrow
			Type	Local
			At	ArrowHit
			Bhvr	WEAPONS\Bow\Archery\ArrowsHitFade.bhvr
			BhvrOverride
				PyrRotate 0 90 0
				PositionOffset -2.4 0 0
			End
			Geom	CrystalArrow
			Lifespan	44
		End

	End

End
