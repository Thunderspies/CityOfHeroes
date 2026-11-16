#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			EName Offset
			Bhvr :Fireworks_Bloom_Projectile.bhvr
		End

		Event
			At Offset
			Type PositOnly
			EName FireworkAnchor
			Part Emotes\Fireworks\Bloom_Head_Sparks_Green.part
			Part Emotes\Fireworks\Bloom_Trail_Sparks_Green.part
			LifeSpan 9
		End

		Event
			At Offset
			Type PositOnly
			EName FireworkTrail
			Part Emotes\Fireworks\Rocket_Head_Trail_Glow_GREEN.part
			Part Emotes\Fireworks\Rocket_Trail_Smoke_GREEN.part
			LifeSpan 8
		End

	End

	Condition
		On Time
		Time 9

		Event
			At FireworkAnchor
			Type Start
			ChildFx :CHILD_Fireworks_Bloom_Green.fx
			EName Firework_Yellow
		End

	End

End
