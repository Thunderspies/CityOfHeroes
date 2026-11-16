###########################################################

FxInfo
LifeSpan 200

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPR_FrigidBlast_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	BP_FrigidBlast_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName 	Smoke01
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Part	:Ravager_FrigidSlash_Claws_SmokeL.part
		Part	:Ravager_FrigidSlash_Claws_SubtractiveL.part
		POther	HandL
		Lifespan 70
	End

	Event
		EName 	Smoke01
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part	:Ravager_FrigidSlash_Claws_SmokeR.part
		Part	:Ravager_FrigidSlash_Claws_SubtractiveR.part
		POther	HandR
		Lifespan 70
	End
End


	Condition
		On Time
		Time 17

		Event
			EName Prime
			At LArmR
			Type Start

			Bhvr :FrigidBlast_Projectile_Large.bhvr
			BhvrOverride
				PositionOffset -1.0 0.0 0.0
			End
			LookAt Target
			Magnet Target
		End

		Event
			EName Trail
			At Prime
			Type Local
			BhvrOverride
				TintGeom	1
				PositionOffset 0.0 0.0 -3.0
			End
			Geom Tintable_FX_IceBolt
		End

		Event
			EName Trail2
			At Prime
			Type Local
			BhvrOverride
				PositionOffset 0.0 0.0 -0.0
			End
			Part :Ravager_FrigidBlast_Glow.part
			Part :Ravager_FrigidBlast_Core.part
			Part :Ravager_FrigidBlast_Glow_Additive.part
			Part :Ravager_FrigidBlast_Snow.part
			Part :Ravager_FrigidBlast_Snow_Additive.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
