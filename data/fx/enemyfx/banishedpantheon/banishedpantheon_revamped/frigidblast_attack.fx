#
FxInfo

	LifeSpan 200

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidBlast_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidBlast_Attack_01 200 100 .8
	End
End
Condition
	On	Time
	Time	0

	Event
		EName 	Smoke01
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_SmokeL.part
		Part	:Defiler_FrigidSlash_Claws_SubtractiveL.part
		POther	HandL
		Lifespan 70
	End


	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_Smoke.part
		Part	:Defiler_FrigidSlash_Claws_Subtractive.part
		POther	HandR
		Lifespan 70
	End
End


	Condition
		On Time
		Time 17

		Event
			At WepL
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :FrigidBlast_Projectile.bhvr
			Geom Tintable_FX_IceBolt
		End

		Event
			At Prime
			Type Local
			EName Trail
			BhvrOverride
				PositionOffset 0.0 0.0 3.0
			End
			Part :FrigidBlast_Glow.part
			Part :FrigidBlast_Core.part
			Part :FrigidBlast_Glow_Additive.part
			Part :FrigidBlast_Snow.part
			Part :FrigidBlast_Snow_Additive.part
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
