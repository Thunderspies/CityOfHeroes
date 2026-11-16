#########################################################
##	General red electricity crawling across characters limbs
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

##LifeSpan	99900

########################################################


Condition

	On 	time
	Time 	0

#	Event
#		Type	StartPositOnly  
#	
#		At	root
#		part1	:mu_CageBarVertThick1.part
#		lifespan 80
#	end


	Event
		
		Type	PositOnly
		At	Hips
		Sound eleccage_loop 100 100 .2
				
	End


	Event
		Type	StartPositOnly  
		Ename	bartop1
		At	root
		bhvr	:mu_CageBarTop1.bhvr
		#Geom    Testing_Target
	end

	
	Event
		Type	StartPositOnly  
	
		At	root
		part1	:mu_CageBarVert1.part
		part2	:mu_CageBarVert2.part
		part3	:mu_CageBarVert3.part
	end

	Event
		Type	StartPositOnly  
		Ename	bar1
		At	root
		bhvr	:mu_CageBar1.bhvr
	end
	

	Event
		Type	local 
		Ename	baroff1
		At	bar1
		bhvr	:mu_CageBarOff1.bhvr
		#Geom    Testing_Target
	end
	
	Event
		Type	local 
	
		At	baroff1
		part1	:mu_CageBar1.part
		part2	:mu_CageBar2.part
		#Geom    Testing_Target
		pmagnet Bartop1
	end



	Event
		Type	StartPositOnly  
		Ename	bar1N
		At	root
		bhvr	:mu_CageBarNeg1.bhvr
	end
	

	Event
		Type	local 
		Ename	baroff1N
		At	bar1N
		bhvr	:mu_CageBarOffNeg1.bhvr
		#Geom    Testing_Target
	end

		
	Event
		Type	local 
	
		At	baroff1N
		part1	:mu_CageBar1.part
		part2	:mu_CageBar2.part
		#Geom    Testing_Target
		pmagnet Bartop1
	end
End

#######################################

Condition

	On 	time
	Time 	70

	Event
		Type	local 
		Ename	bar2
		At	bartop1
		bhvr	:mu_CageBar1.bhvr
	end
	

	Event
		Type	local 
		Ename	baroff2
		At	bar2
		bhvr	:mu_CageBarOff1.bhvr
		#Geom    Testing_Target
	end
	
	Event
		Type	local 
	
		At	baroff2
		part1	:mu_CageBarDown1.part
		part2	:mu_CageBarDown2.part
		#Geom    Testing_Target
		pmagnet Bartop2
	end



	Event
		Type	local 
		Ename	bar2N
		At	bartop1
		bhvr	:mu_CageBarNeg1.bhvr
	end
	

	Event
		Type	local 
		Ename	baroff2N
		At	bar2N
		bhvr	:mu_CageBarOffNeg1.bhvr
		#Geom    Testing_Target
	end

		
	Event
		Type	local 
	
		At	baroff2N
		part1	:mu_CageBarDown1.part
		part2	:mu_CageBarDown2.part
		#Geom    Testing_Target
		pmagnet Bartop2
	end


End


