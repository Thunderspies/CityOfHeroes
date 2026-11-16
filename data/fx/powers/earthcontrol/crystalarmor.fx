#########################################################
##	
##
FxInfo

#Flags	IsArmor


Flags    InheritAlpha
#####################################################################################
##Lens Flares
#####################################################

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		#EName	3
		Type	Local
		At	LarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.2
	
	Event
		#EName	3
		Type	Local
		At	LarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.2
	
	Event
		#EName	3
		Type	Local
		At	UarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.2
	
	Event
		#EName	3
		Type	Local
		At	UarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	

End

Condition
	On 	cycle
	Time 	27
	Chance	.2

	Event
		#EName	3
		Type	Local
		At	LlegL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	28
	Chance	.2

	Event
		#EName	3
		Type	Local
		At	LlegR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End


End

#########################################################################
##########################################################

##Stones Rolling

##########################################################

Condition
	On	Time
	Time 	0
		
	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 .88
	End
End

Condition
	On	Time
	Time 	20
		
	Event
		Type	Local
		At 	origin
		Sound Glow5_loop 100 100 .3
	End
End

Condition
	On	Time
	Time 	18
		
	Event
		Type	Local
		At 	origin
		Sound stone_loop 100 100 .6
	End
End


Condition
	On 	Time
	Time 	3

	Event
		Type	Local 
		At	LLEGL
		
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		#Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local 
		At	LLEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalLLEGL
		
	End

	Event
		Type	Local 
		At	LLEGR
		
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		#Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalLLEGR
		
	End

End

###################################################################


Condition
	On 	Time
	Time 	5

	
	Event
		Type	Local 
		At	ULEGR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalUlegR
		
	End

	Event
		Type	Local 
		At	ULEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalUlegL
		
	End

End
##########################################################

Condition
	On 	Time
	Time 	8


	Event
		Type	Local 
		At	Hips
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalHIPS
		
	End

	
End

#################################################################


Condition
	On 	Time
	Time 	11


	Event
		Type	Local 
		At	chest
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalCHEST
		
	End
	
End

#################################################################


Condition
	On 	Time
	Time 	13


	Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalLARMR
		

	End

	Event
		Type	Local 
		At	LARML
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalLARML
		

	End

End

#################################################################


Condition
	On 	Time
	Time 	15
	Event
		Type	Local 
		At	UARML
		
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		#Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local 
		At	UARML
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalUARML
		
	End

	Event
		Type	Local 
		At	UARMR
		
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		#Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	CrystalUARMR
		
	End

End

###############################################################################

Condition
	On 	death
	
	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part1	:RocksScatter2.part
		Part2	:RocksScatter.part
		Part	:CrystalSparkles.part
		Sound graniteexplo 70 70 .66
		LifeSpan	19
		
	End
	
	

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part3	:StoneArmordirtExplosion.part
		Part1	:StoneArmordirtExplosion1.part
		LifeSpan	7
	End

End

End	