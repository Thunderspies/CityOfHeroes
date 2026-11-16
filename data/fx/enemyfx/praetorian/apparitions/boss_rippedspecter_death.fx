#############################################################
## Steam_High.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Apparitions_Death_01 150 100 .9
		Sound 	Apparitions_Death_02 150 100 .9
		Sound 	Apparitions_Death_03 150 100 .9
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 4.0 0.0
		End
		Part	:Face_Explode.part
		Part	:Explosion_Smoke_Death_Boss.part
		Part	:Explosion_Smoke_Death_Boss_Thick.part
		Part	:Body_Explode_Flash_Large.part
		Part	:Body_Explode_Shockwave.part
		Part	:Explode_Flash_Linger.part
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	0.0 3.0 0.0
		End
		Part	:Body_Explode_Shockwave.part
		Part	:Body_Explode_Flash_Large.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	0.0 3.0 0.0
		End
		Part	:Explosion_Smoke_Death_Boss.part
		Part	:Body_Explode_Shockwave.part
		Part	:Body_Explode_Flash_Large.part
	End
End



Condition
	On 	Time
	Time	0

	Event
		EName	Prime
		Type	Start
		At	Head
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	0.5 0.25 0.5
			Gravity			0.04
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5

		End
		Lifespan 80
	End

	Event
		Ename 	Spinner1
		Type	Local
		At	Prime
		BhvrOverride
			Spin 			0.0 0.4 0.0
			SpinJitter		0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner1
		BhvrOverride
			PositionOffset		0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 80
	End
End


Condition
	On 	Time
	Time	4

	Event
		EName	Prime2
		Type	Start
		At	Head
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	0.5 0.25 0.5
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5
			Gravity			0.04

		End
		Lifespan 80
	End

	Event
		Ename 	Spinner2
		Type	Local
		At	Prime2
		BhvrOverride
			Spin 0.0 .4 0.0
			SpinJitter		0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset		0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 80
	End
End

Condition
	On 	Time
	Time	10

	Event
		EName	Prime3
		Type	Start
		At	Head
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	0.5 0.25 0.5
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5
			Gravity			0.04

		End
		Lifespan 80
	End

	Event
		Ename 	Spinner3
		Type	Local
		At	Prime3
		BhvrOverride
			Spin 0.0 .4 0.0
			SpinJitter	0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner3
		BhvrOverride
			PositionOffset	0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 80
	End
End

Condition
	On 	Time
	Time	15

	Event
		EName	Prime4
		Type	Start
		At	Head
		BhvrOverride
			InitialVelocity		0 1.0 0.0
			InitialVelocityJitter	0.5 0.25 0.5
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5
			Gravity			0.04

		End
		Lifespan 80
	End

	Event
		Ename 	Spinner4
		Type	Local
		At	Prime4
		BhvrOverride
			Spin 0.0 .4 0.0
			SpinJitter	0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner4
		BhvrOverride
			PositionOffset	0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 80
	End
End

#############################################################

End