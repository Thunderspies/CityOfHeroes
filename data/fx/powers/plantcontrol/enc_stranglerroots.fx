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
		Bhvr	:VineFade.bhvr
		geom	vinebig04
	End

	Event
		Ename	vines
		Type	posit 
		At	root
		Bhvr	:VineFade.bhvr
		Part	:VineDust.part
		Part	:VineDust2.part

		LifeSpan	23

	End
End


Condition
	On 	time
	Time 	5

	Event
		Ename	vines1
		Type	posit 
		At	root
		Bhvr	:VineFade1.bhvr
		geom	vinebig03
		
	End

End


Condition
	On 	time
	Time 	7

	Event
		Ename	vines2
		Type	posit 
		At	root
		Bhvr	:VineFade2.bhvr
		geom	vinebig02

	End

End


Condition
	On 	time
	Time 	12

	Event
		Ename	vines3
		Type	posit 
		At	root
		Bhvr	:VineFade3.bhvr
		geom	vinebig01	
	End

End

###########################################################################################

Condition
		
	Event
		Ename	Hookup
		Type	posit 
		At	root
		Bhvr	:spin.bhvr
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
		Bhvr	:StraightVineFade4.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade4.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade3.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade1.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade4.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade1.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade3.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade3.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
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
		Bhvr	:StraightVineFade1.bhvr
		geom	straightvine2
		geom	straightvine
		geom	straightvine3
		
		#LifeSpan	45

	End

End


End			