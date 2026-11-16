#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	chest
End


###########################################################

Condition

	On 	Time
	Time 	0
	
	Event
		EName 	Prime
		Type	start
		At	chest
		Bhvr	behaviors/projectileNOspin2.bhvr
		Geom	TeleBoltShield
		Magnet	Target
		LookAt	Target
		#Part1	:ForceMist01.part
		#Part2	:ForceBallTrail.part
	End

	Event
		EName 	Prime2
		Type	start
		At	chest
		Bhvr	behaviors/projectileSLIGHTspin2.bhvr
		Geom	TeleBolt
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	Prime3
		Type	start
		At	chest
		Bhvr	behaviors/projectileSLIGHTspin2.bhvr
		Geom	TeleBoltTail
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	Prime4
		Type	start
		At	chest
		Bhvr	behaviors/projectileSLIGHTspin2.bhvr
		Geom	ElectricFlares
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	Spark01
		Type	local
		At	Prime4
		altpiv	1
		Part1	:ElectricFlare01.part
		Part2	:ElectricFlare02.part
		Part3	:TeleboltBalls.part
		
	End

	Event
		EName 	Spark01
		Type	local
		At	Prime4
		altpiv	2
		Part1	:ElectricFlare01.part
		Part2	:ElectricFlare02.part
		Part3	:TeleboltBalls.part
		
	End

	Event
		EName 	Spark01
		Type	local
		At	Prime4
		altpiv	3
		Part1	:ElectricFlare01.part
		Part2	:ElectricFlare02.part
		Part3	:TeleboltBalls.part
				
	End

	Event
		EName 	Spark01
		Type	local
		At	Prime4
		altpiv	4
		Part1	:ElectricFlare01.part
		Part2	:ElectricFlare02.part
		Part3	:TeleboltBalls.part
		
	End


End


Condition
	On 	PrimeHit

	Event
		Type    Destroy
		EName 	Prime

	End

	Event
		Type    Destroy
		EName 	Prime2

	End

	Event
		Type    Destroy
		EName 	Prime3

	End

	Event
		Type    Destroy
		EName 	Prime4

	End
End

End
		