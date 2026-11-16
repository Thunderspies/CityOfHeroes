#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	200


Condition

	Event
		EName	SpinningTargetNode
		Type	local
		At	Root
		
		BhvrOverride
			FadeOutLength		220
			StartJitter	22.5 0 22.5
		End
		

	End

	Event
		EName	SpinningTargetNode1
		Type	local
		At	SpinningTargetNode
		
		BhvrOverride
			FadeOutLength		220
			StartJitter	22.5 0 22.5
		End
		

	End

	Event
		EName	SpinningTargetNode0
		Type	local
		At	SpinningTargetNode1
		
		BhvrOverride
			FadeOutLength		220
			StartJitter	22.5 0 22.5
		End
		

	End


End

#############################################################

Condition

	Event
		Ename	Prime2
		Type	start
		At	SpinningTargetNode	
		
		BhvrOverride
			
			FadeOutLength		220

		End

		Bhvr	:LightningBoltBeam.bhvr
		Geom	LightningBoltRigging
		
		Magnet	SpinningTargetNode1	#Target
		LookAt	SpinningTargetNode1	#Target	
	End		

End

Condition

	Event
		Type	local
		At	SpinningTargetNode1
		BhvrOverride
						
			FadeOutLength		220

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node082
	End

End	

Condition

	Event
		Ename	Node082
		Type	local
		At	Prime2
		Altpiv	9
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node072
	End

End	


Condition

	Event
		Ename	Node072
		Type	local
		At	Prime2
		Altpiv	8
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node062
	End

End

Condition

	Event
		Ename	Node062
		Type	local
		At	Prime2
		Altpiv	7
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node052
	End

End

Condition

	Event
		Ename	Node052
		Type	local
		At	Prime2
		Altpiv	6
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node042
	End

End

Condition

	Event
		Ename	Node042
		Type	local
		At	Prime2
		Altpiv	5
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node032
	End

End

Condition

	Event
		Ename	Node032
		Type	local
		At	Prime2
		Altpiv	4
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node022
	End

End

Condition

	Event
		Ename	Node022
		Type	local
		At	Prime2
		Altpiv	3
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node012
	End

End

Condition

	Event
		Ename	Node012
		Type	local
		At	Prime2
		Altpiv	2
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Home2
	End

	Event
		Ename	Home2
		Type	local
		At	Prime2
		altpiv	1
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  SpinningTargetNode	
	End		
	
End


#############################################################################
##Hands
############################################################

Condition
	On 	PrimeHit
	
	Event
		
		EName	Prime2
		Type	destroy
		
		
	End

End

Condition
	On 	Time
	Time 	55
	Event
		
		EName	Thingy2
		Type	destroy
		
		
	End

	Event
		
		EName	Thingy12
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA2
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB2
		Type	destroy
		
		
	End

	

End

########################################################

Condition

	Event
		Ename	Prime1
		Type	start
		At	SpinningTargetNode1	
		
		BhvrOverride
			
			FadeOutLength		220

		End

		Bhvr	:LightningBoltBeam.bhvr
		Geom	LightningBoltRigging
		
		Magnet	SpinningTargetNode0	#Target
		LookAt	SpinningTargetNode0	#Target	
	End		

End

Condition

	Event
		Type	local
		At	SpinningTargetNode0
		BhvrOverride
						
			FadeOutLength		220

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node081
	End

End	

Condition

	Event
		Ename	Node081
		Type	local
		At	Prime1
		Altpiv	9
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node071
	End

End	


Condition

	Event
		Ename	Node071
		Type	local
		At	Prime1
		Altpiv	8
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node061
	End

End

Condition

	Event
		Ename	Node061
		Type	local
		At	Prime1
		Altpiv	7
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node051
	End

End

Condition

	Event
		Ename	Node051
		Type	local
		At	Prime1
		Altpiv	6
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node041
	End

End

Condition

	Event
		Ename	Node041
		Type	local
		At	Prime1
		Altpiv	5
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node031
	End

End

Condition

	Event
		Ename	Node031
		Type	local
		At	Prime1
		Altpiv	4
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node021
	End

End

Condition

	Event
		Ename	Node021
		Type	local
		At	Prime1
		Altpiv	3
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node011
	End

End

Condition

	Event
		Ename	Node011
		Type	local
		At	Prime1
		Altpiv	2
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Home1
	End

	Event
		Ename	Home1
		Type	local
		At	Prime1
		altpiv	1
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  SpinningTargetNode	
	End		
	
End


#############################################################################
##Hands
############################################################

Condition
	On 	PrimeHit
	
	Event
		
		EName	Prime1
		Type	destroy
		
		
	End

End

Condition
	On 	Time
	Time 	55
	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	Thingy11
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB1
		Type	destroy
		
		
	End

	

End
########################################################


Condition

	Event
		Ename	Prime
		Type	start
		At	SpinningTargetNode	
		
		BhvrOverride
			
			FadeOutLength		220

		End

		Bhvr	:LightningBoltBeam.bhvr
		Geom	LightningBoltRigging
		
		Magnet	root	#Target
		LookAt	root	#Target	
	End		

End

Condition

	Event
		Type	local
		At	SpinningTargetNode
		BhvrOverride
						
			FadeOutLength		220

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node08
	End

End	

Condition

	Event
		Ename	Node08
		Type	local
		At	Prime
		Altpiv	9
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node07
	End

End	


Condition

	Event
		Ename	Node07
		Type	local
		At	Prime
		Altpiv	8
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node06
	End

End

Condition

	Event
		Ename	Node06
		Type	local
		At	Prime
		Altpiv	7
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node05
	End

End

Condition

	Event
		Ename	Node05
		Type	local
		At	Prime
		Altpiv	6
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node04
	End

End

Condition

	Event
		Ename	Node04
		Type	local
		At	Prime
		Altpiv	5
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node03
	End

End

Condition

	Event
		Ename	Node03
		Type	local
		At	Prime
		Altpiv	4
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node02
	End

End

Condition

	Event
		Ename	Node02
		Type	local
		At	Prime
		Altpiv	3
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Node01
	End

End

Condition

	Event
		Ename	Node01
		Type	local
		At	Prime
		Altpiv	2
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  Home
	End

	Event
		Ename	Home
		Type	local
		At	Prime
		altpiv	1
		BhvrOverride
			
			StartJitter		.5 .5 .5

		End
		
		Part	:EnergyHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt01.part
		POther  root	
	End		
	
End


#############################################################################
##Hands
############################################################

Condition
	On 	PrimeHit
	
	Event
		
		EName	Prime
		Type	destroy
		
		
	End

End

Condition
	On 	Time
	Time 	55
	Event
		
		EName	Thingy
		Type	destroy
		
		
	End

	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End