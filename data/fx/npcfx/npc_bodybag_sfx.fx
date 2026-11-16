#############################################################
## NPC Movement Whoosh SFX.fx
#############################################################

FxInfo

Lifespan 30

Condition
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Kick_01 100 100 .5
		Sound Kick_02 100 100 .5
		Sound Kick_03 100 100 .5
		Sound Kick_04 100 100 .5
		Sound Kick_05 100 100 .5
	End
End

Condition
	On Time
	Time 10
	Event
		Type Local
		At Origin
		SoundNoRepeat 3
		Sound ChestThump1 50 50 .5
		Sound ChestThump2 50 50 .5
		Sound ChestThump3 50 50 .5
		Sound ChestThump4 50 50 .5
		Sound ChestThump5 50 50 .5
	End
End
Condition
	On Time
	Time 20
	Event
		Type Local
		At Origin
		SoundNoRepeat 3
		Sound ChestThump1 50 50 .75
		Sound ChestThump2 50 50 .75
		Sound ChestThump3 50 50 .75
		Sound ChestThump4 50 50 .75
		Sound ChestThump5 50 50 .75
	End

	Event
		Type Local
		At Origin
		SoundNoRepeat 3
		Sound ClothMovement_Long_01 50 50 .15
		Sound ClothMovement_Long_02 50 50 .15
		Sound ClothMovement_Long_03 50 50 .15
		Sound ClothMovement_Long_04 50 50 .15
	End
End

#############################################################
