#############################################################
## Death_WoodBits.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Start
		At	Chest
		Part	:Smoke_Main.part
		Part	:Smoke_Lingering.part
		Part	:Smoke_Additive.part
		Part	:Smoke_Lingering_Additive.part
		Part	:Sparks.part
		ChildFX	ENEMYFX\BanishedPantheon\Pantheon_Spirits\Death_WoodBits.fx

	End
#############################################################

	Event
		Ename	Rocket1
		Type	Local
		At 	Chest
		Bhvr 	:RocketMotion.bhvr
		Lifespan 3
	End

	Event
		Ename 	Offset1
		Type 	Local
		At	Rocket1
		Part2	:SmokeTrailsAdditive.part
		Part3	:BottleRocketGlows.part
		Lifespan 19
	End
#############################################################

	Event
		Ename	Rocket2
		Type	Local
		At 	Chest
		Bhvr 	:RocketMotion.bhvr
		Lifespan 5
	End

	Event
		Ename 	Offset2
		Type 	Local
		At	Rocket2
		Part2	:SmokeTrailsAdditive.part
		Part3	:BottleRocketGlows.part
		Lifespan 19
	End
#############################################################

	Event
		Ename	Rocket3
		Type	Local
		At 	Chest
		Bhvr 	:RocketMotion.bhvr
		Lifespan 7
	End

	Event
		Ename 	Offset3
		Type 	Local
		At	Rocket3
		Part2	:SmokeTrailsAdditive.part
		Part3	:BottleRocketGlows.part
		Lifespan 19
	End

#############################################################

End

#############################################################

End