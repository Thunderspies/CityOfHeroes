#############################################################
## Child_BeakerShatter.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Mystic
		Part	Explosions\GlassExplosion\GlassDust.part
		Part	Emotes\CostumeChanges\ChemExplosion\SmokeThin.part
		Part	Emotes\CostumeChanges\ChemExplosion\Smoke.part
		Part	Emotes\CostumeChanges\ChemExplosion\Splash.part
		Part	Emotes\CostumeChanges\ChemExplosion\Bubbles.part
		Part	Emotes\CostumeChanges\ChemExplosion\BeakerSparks.part
		LifeSpan  3
	End
End

Condition
	On	Time
	Time	0
	Repeat 	1

	Event
		Type	Local
		At	Mystic
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

Condition
	On 	Time
	Time 	0
	Repeat 	9
	RepeatJitter	2

	Event
		Type	Start
		At	Mystic
		ChildFX Explosions\GlassExplosion\GlassCube.fx
		LifeSpan  150
	End
End

Condition
	On 	Time
	Time 	0
	Repeat 	4
	RepeatJitter	1

	Event
		Type	Start
		At	Mystic
		ChildFX	Explosions\GlassExplosion\GlassShard.fx
		LifeSpan 100
	End
End

#############################################################

End