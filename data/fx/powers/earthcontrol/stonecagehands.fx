#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Lifespan	 21

#########################################################
# AUDIO
#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	local
		At	WepR
		Sound Rumble2 80 80 .7

	End

End

Condition
	On 	Time
	Time 	22
	
	Event
		Type	local
		At	WepR
		Sound PP13 80 80 .8

	End

End

#########################################################
#########################################################



Condition
	On 	Time
	Time 	4
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepR
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		#Part4	:GenericEarthHandsDustRocks.part
		#Part	:GenericEarthHandsDustRocks2.part
		Sound Rumble2 80 80 .7

		LifeSpan	21
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		#Part4	:GenericEarthHandsDustRocks.part
		#Part	:GenericEarthHandsDustRocks2.part

		LifeSpan	21
	End
End

Condition
	On 	Time
	Time 	32
	
	Event
		EName 	HandClapEnergyBall
		Type	local
		At	WepR
		bhvr	behaviors/EarthClapShake.bhvr
		
		LifeSpan	5
	End
End


Condition
	On 	Time
	Time 	22
	
	Event
		EName 	HandClapEnergyBall
		Type	local
		At	WepR
		bhvr	behaviors/ScaleEarthGraspHandHit.bhvr
		Geom	EarthGraspHit

		LifeSpan	5
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepR
		
		Part	:GenericEarthRockScaterLarge.part
		Part	:GenericEarthRockScaterLarge2.part

		LifeSpan	2
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		Part	:GenericEarthRockScaterLarge.part
		Part	:GenericEarthRockScaterLarge2.part

		LifeSpan	2
	End

End

End


