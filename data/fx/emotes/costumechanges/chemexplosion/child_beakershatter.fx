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
		Ename	Center
		Type	Start
		At	Origin
		#Sound	Ice7 100 80 .6
		Part	Explosions\GlassExplosion\GlassDust.part
		Part	:SmokeThin.part
		Part	:Smoke.part
		Part	:Splash.part
		Part	:Bubbles.part
		Part	:BeakerSparks.part
		LifeSpan  3
	End
End
Condition
	On	Time
	Time	0
	Repeat 	1
	Event

		EName	GasSpurts
		Type	Local
		At	chest

		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan	75
	End
End

Condition
	On 	Time
	Time 	0
	Repeat 		9
	RepeatJitter	2

	Event
		Type	Start
		At	center
		ChildFX Explosions\GlassExplosion\GlassCube.fx
		LifeSpan  150
	End
End

Condition
	On 	Time
	Time 	0
	Repeat 		4
	RepeatJitter	1

	Event
		Type	Start
		At	Center
		ChildFX	Explosions\GlassExplosion\GlassShard.fx
		LifeSpan 100
	End
End

#############################################################

End


