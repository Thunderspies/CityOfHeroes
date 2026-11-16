#########################################################
##	
##
FxInfo

##########################################################

Condition
	
	Event
		Ename	vines
		Type	posit 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		Part	CustomizeablePowers\PlantControl\VineDust.part
		Part	CustomizeablePowers\PlantControl\VineDust2.part
		Sound Tuatha_loop 60 60 .66
		#LifeSpan	23

	End
	
	Event
		Ename	Hookup
		Type	Local 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\spin.bhvr
		geom	Centerdummy
				
	End

End


Condition
	On 	Time
	Time	1

	Event
		Ename	vines
		Type	start 
		At	FootR
		Bhvr	CustomizeablePowers\PlantControl\VinespinLeg.bhvr
		geom	vinebig1
		

	End
	
	Event
		Ename	vines
		Type	start 
		At	FootR
		Bhvr	CustomizeablePowers\PlantControl\VinespinLegCounter.bhvr
		geom	vinebig

	End
	
	Event
		Ename	vines
		Type	start 
		At	FootL
		Bhvr	CustomizeablePowers\PlantControl\VinespinLeg.bhvr
		geom	vinebig1
		

	End
	
	Event
		Ename	vines
		Type	start 
		At	FootL
		Bhvr	CustomizeablePowers\PlantControl\VinespinLegCounter.bhvr
		geom	vinebig
		

	End


End


Condition
	#On 	cycle
	#Time 	1
	#Chance	.1

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	1

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	2
	
	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	3
	
	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End
	
End

Condition
	On 	time
	Time 	4

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	5

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	6

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	7

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	8

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	9

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	10

	Event
		Ename	vines
		Type	start 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		geom	straightvine2
		geom	straightvine
		
		#LifeSpan	45

	End

End

Condition
	On 	Time
	Time	1
	

	Event
		Type	start 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\Vinespin.bhvr
		#geom	vine2	
		geom	vinebig1	
		#LifeSpan	10

	End

	Event
		Ename	vines
		Type	start 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VinespinCounter.bhvr
		geom	vinebig	
		#geom	vine1	
		#LifeSpan	10
		
	End

#	Event
#		Ename	vines
#		Type	start 
#		At	root
#		Bhvr	:VinespinCounter.bhvr
#		geom	vinebig1	
#		#geom	vinebig	
#		#LifeSpan	10
#
#	End
#
#	Event
#		Ename	vines
#		Type	start 
#		At	root
#		Bhvr	:Vinespin.bhvr
#		#geom	vinebig1	
#		geom	vinebig	
#		#LifeSpan	10
#	End
#
	Event
		Type	start 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\Vinespina.bhvr
		#geom	vinebig1	
		geom	vinebig		
		#LifeSpan	10

	End

	Event
		Ename	vines
		Type	start 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VinespinCountera.bhvr
		geom	vinebig1	
		geom	vinebig		
		#LifeSpan	10
		
	End

	Event
		Ename	vines
		Type	start 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VinespinCountera.bhvr
		geom	vinebig1	
#		geom	vinebig		
		#LifeSpan	10

	End

	Event
		Ename	vines
		Type	start 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\Vinespina.bhvr
		geom	vinebig1	
#		geom	vinebig		
		#LifeSpan	10
	End

End



End			