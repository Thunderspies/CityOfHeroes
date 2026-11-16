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
		Bhvr	CustomizeablePowers\PlantControl\VineFadea.bhvr
		geom	StranglerVinebig04
		Sound tuatha_emerge 60 60 .7
	End

	Event
		Ename	vines
		Type	posit 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VineFadea.bhvr
		Part	CustomizeablePowers\PlantControl\VineDust.part
		Part	CustomizeablePowers\PlantControl\VineDust2.part
		Sound Tuatha_loop 60 60 .66
		#LifeSpan	23

	End
End


Condition
	On 	time
	Time 	5

	Event
		Ename	vines1
		Type	posit 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VineFade1a.bhvr
		geom	StranglerVinebig03
		
	End

End


Condition
	On 	time
	Time 	7

	Event
		Ename	vines2
		Type	posit 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VineFade2a.bhvr
		geom	StranglerVinebig02

	End

End


Condition
	On 	time
	Time 	12

	Event
		Ename	vines3
		Type	posit 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VineFade3a.bhvr
		geom	StranglerVinebig01	
	End

End

###########################################################################################

Condition
		
	Event
		Ename	Hookup
		Type	posit 
		At	root
		Bhvr	CustomizeablePowers\PlantControl\spin.bhvr
		geom	Centerdummy
				
	End

End

Condition
	On 	time
	Time 	0
	
	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade4.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	2
	
	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End
	
End

Condition
	On 	time
	Time 	3

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade4.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	8

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade3.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	11

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade1.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	15

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade4.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	20

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade1.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	23

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade3.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	24

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade3.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End

Condition
	On 	time
	Time 	30

	Event
		Ename	vines
		Type	posit 
		At	Hookup
		altpiv 1
		Bhvr	CustomizeablePowers\PlantControl\StranglerStraightVineFade1.bhvr
		geom	StranglerStraightvine2
		geom	StranglerStraightvine
		geom	StranglerStraightvine3
		geom	StranglerStraightvine2a
		geom	StranglerStraightvinea
		geom	StranglerStraightvine3a
		geom	StranglerStraightvine2b
		geom	StranglerStraightvineb
		geom	StranglerStraightvine3b
		
		#LifeSpan	45

	End

End


End			