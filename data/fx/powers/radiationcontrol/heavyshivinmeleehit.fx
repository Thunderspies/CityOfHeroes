#########################################################
##	RadiationAura
FxInfo

LifeSpan	100

Input  
	InpName	Origin
End


####################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Prime
		Type	Local
		At	Root
		Geom	OverHeadDummy
		Sound infectexplo2 80 80 .9
		
	End

	Event	
		EName	rays
		Type	Local
		At	Root
	
		Part1	:InfectOthersHITRAYS.part
	End

End


Condition
	On	Time
	Time	5

	Event	
		EName	StemSmoke
		Type	start
		At	Root
		Part1	:InfectOthersStemSmoke.part
		
	End

End


Condition
	On	Time
	Time	11

	Event	
		EName	Rings1
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	11.25

	Event	
		EName	Rings2
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	11.5

	Event	
		EName	Rings3
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	11.75

	Event	
		EName	Rings4
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	12

	Event	
		EName	Rings5
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	12.25

	Event	
		EName	Rings6
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	12.5

	Event	
		EName	Rings7
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	12.75

	Event	
		EName	Rings8
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	13

	Event	
		EName	Rings9
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End
###################################################################



Condition
	On	Time
	Time	21

	Event	
		EName	RingsA
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	21.25

	Event	
		EName	RingsB
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	21.5

	Event	
		EName	RingsC
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	21.75

	Event	
		EName	RingsD
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	22

	Event	
		EName	RingsE
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	22.25

	Event	
		EName	RingsF
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	22.5

	Event	
		EName	RingsG
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	22.75

	Event	
		EName	RingsH
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	23

	Event	
		EName	RingsI
		Type	start
		At	Root
		bhvr    Behaviors/UpWardsMotion2.bhvr
		Part1	:InfectOthersRing.part
	End

End



Condition
	On	Time
	Time	2

	Event
		
		Type	start
		At	Root
			#part1	:Radiationsmoke2.part
		part2	:Radiationflash.part
		part3	:Radiationsmoke.part
		
	End
End



Condition
	On 	Time
	Time 	35

	Event
		EName	StemSmoke
		Type	Destroy
		
	
	End

End



Condition
	On	Time
	Time	17

	Event	
		EName	PlumeSmoke
		Type	start
		At	Prime
		 AltPiv 1
		Part1	:InfectOthersPlumeSmoke.part
		
	End

End

Condition
	On 	Time
	Time 	50

	Event
		EName	Rings1
		Type	Destroy
	
	End

	Event
		EName	Rings2
		Type	Destroy
		
	
	End

	Event
		EName	Rings3
		Type	Destroy
		
	End

	Event
		EName	Rings4
		Type	Destroy
		
	End

	Event
		EName	Rings5
		Type	Destroy
		
	End

	Event
		EName	Rings6
		Type	Destroy
	
	End

	Event
		EName	Rings7
		Type	Destroy
		
	
	End

	Event
		EName	Rings8
		Type	Destroy
	
	End

	Event
		EName	Rings9
		Type	Destroy
		
	
	End
End

#########################################################################

Condition
	On 	Time
	Time 	60

	Event
		EName	RingsA
		Type	Destroy
	
	End

	Event
		EName	RingsB
		Type	Destroy
		
	
	End

	Event
		EName	RingsC
		Type	Destroy
		
	End

	Event
		EName	RingsD
		Type	Destroy
		
	End

	Event
		EName	RingsE
		Type	Destroy
		
	End

	Event
		EName	RingsF
		Type	Destroy
	
	End

	Event
		EName	RingsG
		Type	Destroy
		
	
	End

	Event
		EName	RingsH
		Type	Destroy
	
	End

	Event
		EName	RingsI
		Type	Destroy
		
	
	End
End

Condition
	On 	Time
	Time 	33

	Event
		EName	PlumeSmoke
		Type	Destroy
		
	
	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName	Rays
		Type	Destroy
		
	
	End

End


End

