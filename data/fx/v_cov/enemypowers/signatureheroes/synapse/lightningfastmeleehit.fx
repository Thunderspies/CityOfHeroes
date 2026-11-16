#########################################################
##	template

FxInfo

LifeSpan	120



#############################################################


Condition
	On Time 
	Time 44
	
	Event
		Type 	posit
		At	Hips
		
		Sound sparkscorcher 70 70 .55
	End

End

Condition
	On Time 
	Time 11
	
	Event
		EName	Hits
		Type 	posit
		At	Hips
		
		Part3	POWERS\SuperSpeed\PistolflashStarCircle.part
		Sound flurryhits 50 50 .8
	End

End

Condition
	On Time 
	Time 69
	

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	POWERS\SuperSpeed\Speedflash.part
		Part2	POWERS\SuperSpeed\SpeedStar.part
	End

End

	Condition
	On 	Time
	Time 	85
	Event
		EName 	flash
		Type	Destroy 

	End

End

	Condition
	On 	Time
	Time 	84
	Event
		EName 	Hits
		Type	Destroy 

	End


End

############  Electricity   #################

Condition
	On	Time
	Time	70

	Event
		EName 	HitFX
		Type	local 
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
		#Part	Powers/MartialArts/MartialArtsHitRaysSmall.part
		LifeSpan	10
		
	End

End

Condition
	On	Time
	Time	6

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	POWERS\ElectricityControl\ElectricityGlow3.part
		Part2	POWERS\ElectricityControl\ElectricityGlowStar3.part
		Part3	POWERS\ElectricityControl\ElectricityArch4a.part
		Part4	POWERS\ElectricityControl\ElectricityArchSmalla.part
		Sound pp12 50 50 .7
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	POWERS\ElectricityControl\ElectricityArchContinuing.part
		Part2	POWERS\ElectricityControl\ElectricityArchContinuing2.part
	End
End

	Condition
	On 	Time
	Time 	68
	Event
		EName 	HandGlow1
		Type	Destroy
				
	End

End

Condition
	On Time 
	Time 70

	Repeat	5
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx

		Lifespan	90
		
	End

End

Condition
	On Cycle
	Time 3
	Chance	.8

	Repeat		1
	RepeatJitter	1
	
	Event
		EName	Sparkfar
		Type 	Start
		At	HandGlow1
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90
		
	End

End


End	

	