#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300


#########################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Origin
		Sound ghostblast 120 120 .95
	End
End


Condition
	On 	Time
	Time 	22

	Event
		EName	Prime
		Type	start
		At	chest
		Bhvr	behaviors/Skullprojectiles.bhvr
		Magnet	Target
		LookAt  Target
		
	
	End

	Event
		EName	SkullRotate
		Type	local
		At	Prime
		Bhvr	behaviors/SkullprojectilesRotate.bhvr
			
	
	End

	Event
		EName	SkullOffset
		Type	local
		At	SkullRotate
		Bhvr	behaviors/SkullprojectilesRotate.bhvr
		Geom	Skull
		
		
	
	End
	
	Event
		EName	eye1
		Type	local
		At	SkullOffset
		 altpiv	2
		Part1	:DaemonEyes2.part
		Part2	:DaemonPupil2.part
	End

	Event
		EName	eye2
		Type	local
		At	SkullOffset
		 altpiv	3
		Part1	:DaemonEyes2.part
		Part2	:DaemonPupil2.part
	End


	Event
		Type	local
		At	SkullOffset
		altpiv	1

		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:Skullsmoke.part
		Part	:Skullsmoke.part
			
	End

End

##########################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	Prime1
		Type	start
		At	chest
		Bhvr	behaviors/Skullprojectiles1.bhvr
		Magnet	Target
		LookAt  Target
		
	
	End

	Event
		EName	SkullRotate1
		Type	local
		At	Prime1
		Bhvr	behaviors/SkullprojectilesRotate1.bhvr
			
	
	End

	Event
		EName	SkullOffset1
		Type	local
		At	SkullRotate1
		Bhvr	behaviors/SkullprojectilesRotate1.bhvr
		Geom	Skull
		
		
	
	End
	
	Event
		EName	eye1
		Type	local
		At	SkullOffset1
		 altpiv	2
		Part1	:DaemonEyes2.part
		Part2	:DaemonPupil2.part
	End

	Event
		EName	eye2
		Type	local
		At	SkullOffset1
		 altpiv	3
		Part1	:DaemonEyes2.part
		Part2	:DaemonPupil2.part
	End


	Event
		Type	local
		At	SkullOffset1
		altpiv	1
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:Skullsmoke.part
		Part	:Skullsmoke.part
			
	End

End

###################################################################
#
#########################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	Prime2
		Type	start
		At	chest
		Bhvr	behaviors/Skullprojectiles2.bhvr
		Magnet	Target
		LookAt  Target
		
	
	End

	Event
		EName	SkullRotate2
		Type	local
		At	Prime1
		Bhvr	behaviors/SkullprojectilesRotate2.bhvr
			
	
	End

	Event
		EName	SkullOffset2
		Type	local
		At	SkullRotate2
		Bhvr	behaviors/SkullprojectilesRotate2.bhvr
		Geom	Skull
		
		
	
	End
	
	Event
		EName	eye1
		Type	local
		At	SkullOffset2
		 altpiv	2
		Part1	:DaemonEyes2.part
		Part2	:DaemonPupil2.part
	End

	Event
		EName	eye2
		Type	local
		At	SkullOffset2
		 altpiv	3
		Part1	:DaemonEyes2.part
		Part2	:DaemonPupil2.part
	End


	Event
		Type	local
		At	SkullOffset2
		altpiv	1

		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticlesSubtractive.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:SkullTenticles.part
		Part	:Skullsmoke.part
		Part	:Skullsmoke.part
					
	End

End

#################################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime1
		Type	Destroy

	End

	Event
		EName 	Prime1
		Type	Destroy

	End

End


End	


			