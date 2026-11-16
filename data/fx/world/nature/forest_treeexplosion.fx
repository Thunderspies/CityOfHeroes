#############################################################
## Forest_TreeExplosion.fx
#############################################################

FxInfo
Lifespan 350

#############################################################

Condition
	Event
		Type	Local
		At Origin
		SoundNoRepeat 1
		Sound Hamidon_Tree_01 6000 5000 .8
		Sound Hamidon_Tree_02 6000 5000 .8
	End
End

Condition
	On Time
	Time 60
	
	Event
		Type Local
		At Origin
		SoundNoRepeat 1
		Sound Hamidon_TentacleVox_01 6000 5000 .8
		Sound Hamidon_TentacleVox_02 6000 5000 .8
		Sound Hamidon_TentacleVox_03 6000 5000 .8
	End
End



Condition
	On	Time
	Time	0
	
	Event
		EName	DirtAnchor
		Type	Create
		Inherit	Position

		BhvrOverride
			PositionOffset	0.0 80.0 0.0
		End
	End

	Event
		Type	Start
		At	DirtAnchor

		Part	:Dust_Flash.part
		Part	:Dust_Flash2.part
		Lifespan	8
	End


	Event
		Type	Start
		At	DirtAnchor
		BhvrOverride
			PositionOffset	0 180 0
		End
		Part	:Rocks.part
	End
End

Condition
	On	Time
	Time	0
	Chance	0.3
	Event
		Type	Local
		At	DirtAnchor
		BhvrOverride
			PositionOffset		0.0 100.0 0.0
			StartJitter		.75 .75 .75
			Scale			4.0 4.0 4.0
			InitialVelocity		5.0 20.2 5.0
			InitialVelocityJitter	2.3 2.2 2.3
			PyrRotateJitter		180 180 180
			SpinJitter		0.05 0.05 0.05
			Gravity			0.4
		End

		Geom	_P_Tree_OakLgA
		CThresh 	.001
		CDestroy 	1
		Lifespan	180
	End
End


Condition
	On	Time
	Time	20

	Event
		Type	Start
		At	DirtAnchor
		BhvrOverride
			PositionOffset	0 180 0
		End
		Part	:Dust_Pillar.part
		Part	:Dirt_Pillar.part
	End
End

#############################################################

End
