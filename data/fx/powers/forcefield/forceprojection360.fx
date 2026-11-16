#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	150

Input  
	Inpname	Root
End

Input  
	Inpname	Hips
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Ball
		Type	Local 
		At	root
		
		Bhvr 	behaviors/ForceBall.bhvr
		
		Geom	ForceBall
				
	End

	Event
		EName 	Ring01
		Type	Local 
		At	Root
		Bhvr 	behaviors/ForceRing02.bhvr
		
		Geom	ForceRing02
						
	End


	Event
		EName 	Ring02
		Type	Local 
		At	Root
		Bhvr 	behaviors/ForceRing01.bhvr
		
		Geom	ForceRing02
						
	End
End

Condition
	On 	Time
	Time 	10


	Event
		EName	InitialSwordFire
		Type	Local 
		At	Hips
		Part1	:Force360Flash.part
		Part2	:Force360Ember.part
		Part3	:Force360Streaks.part
		Part4	:Force360Rays.part
		Part5	:Force360Ring.part
	End


	Event
		EName 	BlastWave
		Type	start
		At	Hips
		
		Bhvr 	behaviors/WaveScale.bhvr
		
		Geom	ForceWave
				
	End
End

Condition
	On 	Time
	Time 	20

	

	Event
		EName 	ParticleBlastWave
		Type	start
		At	Hips
		
		#Bhvr 	behaviors/WaveScale.bhvr
		
		Part1	:BubbleSphereRingExplosion.part				
	End
	
	
	Event
		EName 	Ring03
		Type	Local 
		At	Root
		Bhvr 	behaviors/ForceRing02.bhvr
		
		Geom	ForceRing02
						
	End


	Event
		EName 	Ring04
		Type	Local 
		At	Root
		Bhvr 	behaviors/ForceRing01.bhvr
		
		Geom	ForceRing02
						
	End		

End

#################################################################################

Condition
	On 	Time
	Time 	47

	Event
		EName 	BlastWave
		Type	Destroy				
	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName 	Ring
		Type	Destroy				
	End

	
End

Condition
	On 	Time
	Time 	100

	Event
		EName 	Ring01
		Type	Destroy				
	End

	Event
		EName 	Ring02
		Type	Destroy				
	End

	Event
		EName 	Ring03
		Type	Destroy				
	End

	Event
		EName 	Ring04
		Type	Destroy				
	End

	Event
		EName 	Ball
		Type	Destroy				
	End

End

Condition
	On 	Time
	Time 	70

	Event
		EName 	RingBlast
		Type	Destroy				
	End

	
End


Condition
	On 	Time
	Time 	150

	Event
		EName 	all
		Type	Destroy				
	End

	
End


End