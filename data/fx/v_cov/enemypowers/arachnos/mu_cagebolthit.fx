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

LifeSpan	200

########################################################


Condition

	On 	time
	Time 	0



	Event
		Type	StartPositOnly  
		Ename	bartop1
		At	root
		bhvr	:mu_CageBarTop1.bhvr
		#Sound	mu_cageboltshit 80 70 .88
		#Geom    Testing_Target
	end

	
	Event
		Type	StartPositOnly  
	
		At	root
		part1	:mu_CageBarVert1.part
		part2	:mu_CageBarVert1.part
		part3	:mu_CageBarVert1.part
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
		part2	:mu_CageBar1.part
#		part3	:mu_CageBar1.part
#		part4	:mu_CageBar1.part
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
		part2	:mu_CageBar1.part
#		part3	:mu_CageBar1.part
#		part4	:mu_CageBar1.part
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
		part2	:mu_CageBarDown1.part
#		part3	:mu_CageBarDown1.part
#		part4	:mu_CageBarDown1.part
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
		part2	:mu_CageBarDown1.part
#		part3	:mu_CageBarDown1.part
#		part4	:mu_CageBarDown1.part
		#Geom    Testing_Target
		pmagnet Bartop2
	end


End


