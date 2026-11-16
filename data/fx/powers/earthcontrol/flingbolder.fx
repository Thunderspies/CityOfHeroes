#########################################################
##	Fireweapon
########################################################
FxInfo

######################################################################

Condition
	
	Event
		EName	Streak1
		Type	local
		At	WepR
		
		Sound rockpickup7 100.0 100.0 .85
	End


######################################################################



	Event
		EName	cracks
		Type	start
		At	root
		bhvr	behaviors/CameraShakeBoulderHurl.bhvr
		Part	:EarthWeaponDustRocks2.part		
		Part2	:superStrengthDust1.part
		part3	:superStrengthDust2.part
	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName	crack
		Type	start
		At	root
				
		Part1	:StalagtiteCrack5.part
	End

	Event
		EName	concrete
		Type	local
		At	root
		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05
		bhvr	behaviors/projectileFlingBolder.bhvr
		
		lifespan	16	
				
	End

	Event
		EName	Streak1
		Type	local
		At	root
		part1	:EarthHands.part
		part2	:EarthHands2.part
		part	:EarthHandsRocks.part
		lifespan	25
	End
End

Condition
	On 	Time
	Time 	16

	Event

		EName 	Prime
		Type	Start 
		At	concrete
		
		geom	Parent
		Bhvr	behaviors/projectileFlingBolder2.bhvr
		Magnet	Target
		LookAt	Target
		Sound Rockthrow 100 100 .9
	
		
	End

End

Condition
	On 	Time
	Time 	95

	Event
		EName	concrete
		Type	Destroy
		
	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End
	

End

Condition
	On 	Time
	Time 	60

	Event
		EName	cracks
		Type	Destroy
		
	End
	

End

Condition
	On 	Time
	Time 	200

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

