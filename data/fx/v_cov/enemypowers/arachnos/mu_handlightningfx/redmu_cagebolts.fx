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
	Time 	20


	Event
		Type	local 
		At	origin
		Sound mu_cageboltramp 120 120 .9
#		lifespan 20
	End

end

Condition
	On 	Time
	Time 	40


	Event
		Type	local 
		At	origin
		Sound mu_cagebolts 120 120 .9
#		lifespan 20
	End

	end
	
Condition
	On 	Time
	Time 	20
	Event
		Ename	lift
		Type	local
	
		At	chest
		bhvr V_COV\EnemyPowers\Arachnos\mu_CageLift.bhvr
		lifespan 35
		magnet baroff1N
		#geom    Testing_Target
	End

	Event
		Ename	spotR
		Type	local
	
		At	chest
		bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTrackerR.bhvr
		lifespan 35
		magnet baroff1N
		#geom    Testing_Target
	End

	Event
		Ename	spotL
		Type	local
	
		At	chest
		bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTrackerL.bhvr
		lifespan 35
		magnet baroff1
		#geom    Testing_Target
	End

	#################
	##Reds
	###################################
	Event
		Ename	HandR
		Type	local
		At	wepL
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs.part
		lifespan  15
	End
		
	Event
		Ename	HandL
		Type	local
		At	wepR
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs.part
		lifespan  15
	End
	
	#################
	##Whites
	###################################
	Event
		Ename	HandR
		Type	local
		At	wepL
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs_WhiteSubtle.part
		lifespan  25
	End
		
	Event
		Ename	HandL
		Type	local
		At	wepR
		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs_WhiteSubtle.part
		lifespan  25
	End

End


Condition
	On 	Time
	Time 	40



	Event
		Type	local 
		At	origin
		#Sound	firezap 100 30 .7
#		lifespan 20
	End


	Event
		
		Type	start
		At	HandL

		Part1	V_COV\EnemyPowers\Arachnos\mu_CageRing1.part
		lifespan	5
	End
	Event
		
		Type	start
		At	HandR

		Part1	V_COV\EnemyPowers\Arachnos\mu_CageRing1.part
		lifespan	5
	End





	Event
		Type	positonly  
		Ename	bar
		At	t_root
		bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBar1.bhvr
	end

	
	Event
		Type	local  
		Ename	bar1
		At	bar
		bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBar1.bhvr
	end
	
	Event
		Type	local 
		Ename	baroff1
		At	bar1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBarOff1.bhvr
		#geom    Testing_Target
	end
	

	Event
		Type	local 
		Ename	baroff1N
		At	bar1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_CageBarOffNeg1.bhvr
		#geom    Testing_Target
	end


####



	Event
		Ename	trackerR
		Type	start
	
		At	spotR
		bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTracker.bhvr
		lifespan 35
		magnet baroff1N
		#geom    Testing_Target
	End

	Event
		Type	local
	
		At	trackerR
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock2.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShockSpark.part
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageShockSpark_White.part

		lifespan 35
		#geom    Testing_Target
	End

	


	Event
		Ename	trackerL
		Type	start
	
		At	spotL
		bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTracker.bhvr
		lifespan 35
		magnet baroff1
		#geom    Testing_Target
	End


	Event
		Type	local
	
		At	trackerL
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock1.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShock2.part
		part1 V_COV\EnemyPowers\Arachnos\mu_CageShockSpark.part
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageShockSpark_White.part
		lifespan 35
		#geom    Testing_Target
	End




End
#
#Condition	#### Lightning arcs to "activate" the cage
#	On 	Time
#	Time 	20
#
#	Event
#		Type	local
#	
#		At	neck 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	neck 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#	Event
#		Type	local
#	
#		At	chest 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	chest 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#	Event
#		Type	local
#	
#		At	hips 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	hips 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#end
#
#Condition
#	On 	Time
#	Time 	24
#
#	Event
#		Type	local
#	
#		At	neck 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	neck 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#	Event
#		Type	local
#	
#		At	chest 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	chest 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#	Event
#		Type	local
#	
#		At	hips 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	hips 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#end

#Condition
#	On 	Time
#	Time 	28
#
#	Event
#		Type	local
#	
#		At	neck 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	neck 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#	Event
#		Type	local
#	
#		At	chest 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	chest 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#	Event
#		Type	local
#	
#		At	hips 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotR
#	End
#	Event
#		Type	local
#	
#		At	hips 
#		part1 V_COV\EnemyPowers\Arachnos\mu_CageReacher1.part
#		lifespan 6
#		pother spotL
#	End
#
#	end
#
	
Condition  #######explosion on trackers when the attack needs to hit
	On 	time
	Time 	55


	Event
		Type	local
	
		At	baroff1 
		part1 V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst1Subtle.part
		part2 V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst2Subtle.part
		part3 V_COV\EnemyPowers\Arachnos\mu_CageFence3Subtle.part
		lifespan 15
	End
	Event
		Type	local
	
		At	baroff1N 
		part1 V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst1Subtle.part
		part2 V_COV\EnemyPowers\Arachnos\mu_CageFenceBurst2Subtle.part
		part3 V_COV\EnemyPowers\Arachnos\mu_CageFence3Subtle.part
		lifespan 15
	End

	end


