#########################################################
##	RadiationAura
FxInfo

LifeSpan	120

Input  
	InpName	Root
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	Inpname	Origin
End

Input  
	Inpname	Mystic
End


####################################################################


Condition
	On 	Time
	Time 	0

	Event
		EName	Electrons
		Type	local 
		At	WepL
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
		
	End

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .4
		LifeSpan	120
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		LifeSpan	120
	End

End

Condition
	On	Time
	Time	50

	Event	
		EName	Prime
		Type	Local
		At	Root
		Geom	OverHeadDummy
		
	End

End


Condition
	On	Time
	Time	45

	Event	
		EName	StemSmoke
		Type	start
		At	Root
		Part1	:InfectOthersStemSmoke.part
		
	End

End

Condition
	On	Time
	Time	29

	Event
		Type	start
		At	Mystic  ##Origin
		Sound atomicblast3 100 100 1.0
		
	End
End


Condition
	On	Time
	Time	41

	Event	
		EName	Rings1
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
		#Sound	atomicblast3 80 30 .88
	End

End

Condition
	On	Time
	Time	41.25

	Event	
		EName	Rings2
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	41.5

	Event	
		EName	Rings3
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	41.75

	Event	
		EName	Rings4
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	42

	Event	
		EName	Rings5
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	42.25

	Event	
		EName	Rings6
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	42.5

	Event	
		EName	Rings7
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	42.75

	Event	
		EName	Rings8
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End

Condition
	On	Time
	Time	43

	Event	
		EName	Rings9
		Type	start
		At	Mystic  ##Root
		bhvr    Behaviors/UpWardsMotion.bhvr
		Part1	:InfectOthersRing.part
	End

End
###################################################################



Condition
	On	Time
	Time	61

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
	Time	61.25

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
	Time	61.5

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
	Time	61.75

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
	Time	62

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
	Time	62.25

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
	Time	62.5

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
	Time	62.75

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
	Time	63

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
	Time	42

	Event
		
		Type	start
		At	Root
		part1	:RadiationflashThinRing.part
		part2	:Radiationflash2.part
		part3	:Radiationsmoke.part
		Bhvr	Behaviors/CameraShake01.bhvr
		
	End
End

Condition
	On 	Time
	Time 	75

	Event
		EName	StemSmoke
		Type	Destroy
		
	
	End

End



Condition
	On	Time
	Time	55

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
	Time 	90

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
	Time 	100

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
	Time 	83

	Event
		EName	PlumeSmoke
		Type	Destroy
		
	
	End

End


End

