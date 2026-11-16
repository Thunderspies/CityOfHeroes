#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 100

#####  Fake lob target  #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Fake
		Type	Start
		At	root  ##Origin
		Geom	Lob_base
	End


	Event
		EName 	target_A
		Type	Start
		At	Fake
		Altpiv 3
####		Geom	FX_MachineGunBurst
	End


End


#####  projectile  #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	WepR
		geom	grenade
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		Magnet	target_A
		LookAt	target_A

	End
End

#####  impact  #######################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	fire
		Type	Start						#3 choices, Start (stays in locaion of object at start frame), Local, or Posit 
		At	Fake
		Altpiv 3						#node on skel to attach fire
		Bhvr	behaviors\Fire_offset.bhvr
		Part1	WORLD/Ruins/Mediumfire_flame_A.part
		Part2	world/Ruins/Mediumfire_flame_B.part	
		Part3	WORLD/Ruins/Mediumfire_flame_Flat.part
##		Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/mediumfire_Smoke.part
		Sound torch2_loop 35 35 .33
	End


End

End