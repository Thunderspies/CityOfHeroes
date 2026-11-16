#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Mystic
End

Input  
	InpName	Target
End    

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

#########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName	InitialSwordFire2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
	End


End


Condition
	On 	Time
	Time 	30

	Event
		EName	InitialSwordFire
		Type	Destroy
		
	
	End

End


Condition
	On 	Time
	Time 	30

	Event
		EName	InitialSwordFire2
		Type	Destroy
		
	
	End

End

######################################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	Posit
		At	Mystic
		Bhvr	behaviors/Powers/EnergyBlast/EnergyBlastBeam.bhvr
		Geom	EnergyBlast1
		Magnet	Target
		LookAt	Target
	End	

	Event
		EName 	cone
		Type	Local
		Bhvr	behaviors/Powers/EnergyBlast/EnergyBlastHead.bhvr
		At	Prime
	         AltPiv 1
		Geom	EnergyBlastCone1
		POther	Target
		PMagnet	Target
	End

	Event
		EName 	cone2
		Type	Local
		Bhvr	behaviors/Powers/EnergyBlast/EnergyBlastHeadInner.bhvr
		At	Prime
	         AltPiv 1
		Geom	EnergyBlastCone2
		POther	Target
		PMagnet	Target
	End
End



Condition
	On 	Time
	Time 	20

	Event
		EName 	bob
		Type	Posit
		At	Mystic
		Bhvr	behaviors/Powers/EnergyBlast/EnergyBlastBeamSmall.bhvr
		Geom	EnergyBlast2
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Thingy
		Type	Posit
		At	Origin
		
		#Part1	powers/energyblast/Eblastcharge.part
	End
End

Condition
	On 	PrimeBeamHit
	Event
		EName 	Prime
		Type	Destroy
	
	End

	Event
		EName 	Bob
		Type	Destroy
	
	End

	Event
		EName 	Cone
		Type	Destroy
	End
	
	Event
		EName 	Cone2
		Type	Destroy
	End

	Event
		Ename	Hit1
		Type	Posit
		At	target
		part1	:RadiationElectronsHit.part
		part2	:RadiationBubbleDotsHit.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		
		PMagnet	Hips
		
	End

	Event
		Ename	Hit2
		Type	Posit
		At	target
		part2	:RadiationAuraGreenHit.part
		part3	:RadiationAuraYellowHit.part
		
		
	End

	Event
		Ename	lightrays
		Type	Posit
		At	target

		part1	:RadiationLightHits.part

	End

End

	Condition
	On	Time
	Time	145
	Event
		EName 	lightrays
		Type	Destroy
	End



End


End	


			