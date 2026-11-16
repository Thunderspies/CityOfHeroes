#
FxInfo

	Flags DontSuppress

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ShadowHound_Shadowfire_Attack 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName Anchor1
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Anchor1
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :Projectile_WithUp.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Shadowfire_projectile_Dark.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Shadowfire_projectilemiddle.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Shadowfire_projectile.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Shadowfire_projectile2.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Shadowfire_projectile3.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At T_Chest
			Type Local
			#ChildFx :Judgment_Pyronic_Hit.fx
		End

	End

End
