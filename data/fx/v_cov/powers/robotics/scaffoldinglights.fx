#########################################################
##	template

FxInfo

LifeSpan		260

###################################################################################
##Electricity Action
#########################################################################3

Condition
	On	cycle
	Time	10
	
	
	Event	
		ENAME	electricityhookup
		Type	local
		At	Root
		
		BHVR	:ScaffoldingScaleElectricty.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStar.part
		LifeSpan	100
	End

	Event	
		ENAME	electricityhookup1
		Type	local
		At	Root
		
		BHVR	:ScaffoldingScaleElectricty1.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStar.part
		LifeSpan	100
	End
	
	Event	
		ENAME	electricityhookup2
		Type	local
		At	Root
		
		BHVR	:ScaffoldingScaleElectricty2.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStar.part
		LifeSpan	100
	End
	
	Event	
		ENAME	electricityhookup3
		Type	local
		At	Root
		
		BHVR	:ScaffoldingScaleElectricty3.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStar.part
		LifeSpan	100
	End
End

###################################################################################
##Electricity Action Outside
#########################################################################3

Condition
	On	cycle
	Time	3
	
	
	Event	
		ENAME	electricityhookup
		Type	local
		At	Root
		
		BHVR	:OutsideScaffoldingScaleElectricty.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStarRed.part
		LifeSpan	20
	End

	Event	
		ENAME	electricityhookup1
		Type	local
		At	Root
		
		BHVR	:OutsideScaffoldingScaleElectricty1.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStarRed.part
		LifeSpan	20
	End
	
	Event	
		ENAME	electricityhookup2
		Type	local
		At	Root
		
		BHVR	:OutsideScaffoldingScaleElectricty2.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStarRed.part
		LifeSpan	20
	End
	
	Event	
		ENAME	electricityhookup3
		Type	local
		At	Root
		
		BHVR	:OutsideScaffoldingScaleElectricty3.bhvr
		Geom	centerdummy
		Part	:ElectricityArchStarRed.part
		LifeSpan	20
	End
End

#################################################################################
##ElectricityStricks
#############################################

Condition
	On	cycle
	Time	7
	Chance	.7

	Event	
		Type	local
		At	electricityhookup
		Altpiv	1
		#BHVR	:ScaffoldingScalekittycornerOpposite.bhvr
		
#		Part	:ChargedBolt01.Part
#		Part	:ChargedBolt02.Part

#		Part	:ChargedBolt03.Part
#		Part	:ChargedBolt04.Part

		Part	:ChargedBolt05.Part
		Part	:ChargedBolt06.Part

		POther  head

		LifeSpan	171	
			
	End
End	

Condition
	On	cycle
	Time	7
	Chance	.7

	Event	
		Type	local
		At	electricityhookup1
		Altpiv	1
		#BHVR	:ScaffoldingScalekittycornerOpposite.bhvr
		
#		Part	:ChargedBolt01.Part
#		Part	:ChargedBolt02.Part

		Part	:ChargedBolt03.Part
		Part	:ChargedBolt04.Part

#		Part	:ChargedBolt05.Part
#		Part	:ChargedBolt06.Part

		POther  head

		LifeSpan	171	
			
	End
End	


Condition
	On	cycle
	Time	7
	Chance	.7

	Event	
		Type	local
		At	electricityhookup2
		Altpiv	1
		#BHVR	:ScaffoldingScalekittycornerOpposite.bhvr
		
		Part	:ChargedBolt01.Part
		Part	:ChargedBolt02.Part

#		Part	:ChargedBolt03.Part
#		Part	:ChargedBolt04.Part

#		Part	:ChargedBolt05.Part
#		Part	:ChargedBolt06.Part

		POther  head

		LifeSpan	171	
			
	End
End	

Condition
	On	cycle
	Time	7
	Chance	.7

	Event	
		Type	local
		At	electricityhookup3
		Altpiv	1
		#BHVR	:ScaffoldingScalekittycornerOpposite.bhvr
		
		Part	:ChargedBolt01.Part
#		Part	:ChargedBolt02.Part

#		Part	:ChargedBolt03.Part
#		Part	:ChargedBolt04.Part

#		Part	:ChargedBolt05.Part
		Part	:ChargedBolt06.Part

		POther  head

		LifeSpan	171	
			
	End
End	

End
