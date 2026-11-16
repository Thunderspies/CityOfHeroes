#########################################################
##	forceBubble
########################################################
FxInfo


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	hips
				
		#Sound	shackleshit 70 60 .6
	End

	On 	Time
	Time 	0

	Event
		Type	Local
		At	hips
				
		#Sound	eleccage_loop 70 60 .4
	End

	Event
		EName	HandGlow1
		Type	Local
		At	hips
		Part2	POWERS\ElectricityControl\ElectricityGlowStar2.part
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part2	POWERS\ElectricityControl\ElectricityGlowStar2.part
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
		

	End

	Event
		EName	HandGlow1
		Type	Local
		At	Head

		Part	POWERS\ElectricityControl\ElectricityGlowStarSmall.part
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part

	End
	
	Event
		EName	continuingArchs
		Type	Local
		At	hips
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	UlegL
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	UlegR
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	LlegR
		Part	POWERS\ElectricityControl\ElectricityGlowStarSmall.part
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	LlegL
		Part	POWERS\ElectricityControl\ElectricityGlowStarSmall.part
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootL
		Part	POWERS\ElectricityControl\ElectricityGlowStarSmall.part
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootR
		Part	POWERS\ElectricityControl\ElectricityGlowStarSmall.part
		Part3	POWERS\ElectricityControl\ShaklesElectricityArch2.part
	End

End
	
End