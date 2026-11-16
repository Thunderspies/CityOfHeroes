#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	100

########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	local 
		At	origin
		Sound mu_cageboltramp 120 120 .9
#		lifespan 20
	End

end

Condition
	On 	Time
	Time 	20


	Event
		Type	local 
		At	origin
		Sound mu_cagebolts 120 120 .9
#		lifespan 20
	End

	end
	
Condition
	On 	Time
	Time 	0
	Event
		Ename	lift
		Type	local
	
		At	root
		bhvr :mu_CageLift.bhvr
		lifespan 35
#		magnet baroff1N
		#geom    Testing_Target
	End
	Event
		Ename	spotR
		Type	local
	
		At	lift
		bhvr :mu_CageShockTrackerR.bhvr
		lifespan 35
#		magnet baroff1N
		#geom    Testing_Target
	End

	Event
		Ename	spotL
		Type	local
	
		At	lift
		bhvr :mu_CageShockTrackerL.bhvr
		lifespan 35
#		magnet baroff1
		#geom    Testing_Target
	End

	Event
		Type	local
		At	spotR
		Part1	:mu_Cage_Redball1.part
		lifespan  15
	End

		Event
		Type	local
		At	spotL
		Part1	:mu_Cage_Redball1.part
		lifespan  15
	End

end


Condition
	On 	Time
	Time 	20



	Event
		Type	local 
		At	origin
		#Sound	firezap 100 30 .7
#		lifespan 20
	End


	Event
		
		Type	start
		At	spotL

		Part1	:mu_CageRing1.part
		lifespan	5
	End
	Event
		
		Type	start
		At	spotR

		Part1	:mu_CageRing1.part
		lifespan	5
	End





	Event
		Type	positonly  
		Ename	bar
		At	t_root
		bhvr	:mu_CageBar1.bhvr
	end

	
	Event
		Type	local  
		Ename	bar1
		At	bar
		bhvr	:mu_CageBar1.bhvr
	end
	
	Event
		Type	local 
		Ename	baroff1
		At	bar1
		bhvr	:mu_CageBarOff1.bhvr
		#geom    Testing_Target
	end
	

	Event
		Type	local 
		Ename	baroff1N
		At	bar1
		bhvr	:mu_CageBarOffNeg1.bhvr
		#geom    Testing_Target
	end


####



	Event
		Ename	trackerR
		Type	start
	
		At	spotR
		bhvr :mu_CageShockTracker.bhvr
		lifespan 35
		magnet baroff1N
		#geom    Testing_Target
	End

	Event
		Type	local
	
		At	trackerR
		part1 :mu_CageShockVert1.part
		part1 :mu_CageShockVert2.part
		part1 :mu_CageShockSpark1.part
		lifespan 35
		#geom    Testing_Target
	End

	


	Event
		Ename	trackerL
		Type	start
	
		At	spotL
		bhvr :mu_CageShockTracker.bhvr
		lifespan 35
		magnet baroff1
		#geom    Testing_Target
	End


	Event
		Type	local
	
		At	trackerL
		part1 :mu_CageShockVert1.part
		part1 :mu_CageShockVert2.part
		part1 :mu_CageShockSpark1.part
		lifespan 35
		#geom    Testing_Target
	End




End

Condition	#### Lightning arcs to "activate" the cage
	On 	Time
	Time 	0

	Event
		Type	local
	
		At	neck 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	neck 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	Event
		Type	local
	
		At	chest 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	chest 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	Event
		Type	local
	
		At	hips 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	hips 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	end
Condition
	On 	Time
	Time 	4

	Event
		Type	local
	
		At	neck 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	neck 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	Event
		Type	local
	
		At	chest 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	chest 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	Event
		Type	local
	
		At	hips 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	hips 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	end
Condition
	On 	Time
	Time 	8

	Event
		Type	local
	
		At	neck 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	neck 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	Event
		Type	local
	
		At	chest 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	chest 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	Event
		Type	local
	
		At	hips 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotR
	End
	Event
		Type	local
	
		At	hips 
		part1 :mu_CageReacher1.part
		lifespan 6
		pother spotL
	End

	end

	
Condition  #######explosion on trackers when the attack needs to hit
	On 	time
	Time 	35


	Event
		Type	local
	
		At	baroff1 
		part1 :mu_CageShockBurst1.part
		part2 :mu_CageShockBurst2.part
		part3 :mu_CageShockBurst3.part
		lifespan 15
	End
	Event
		Type	local
	
		At	baroff1N 
		part1 :mu_CageShockBurst1.part
		part2 :mu_CageShockBurst2.part
		part3 :mu_CageShockBurst3.part
		lifespan 15
	End

	end


