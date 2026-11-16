#########################################################
##	Predefined arc curve
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

LifeSpan	30

########################################################


Condition

	On 	time
	Time 	0
	#	Chance	.1
	Event
		Type	Local
	
		At	root
		childfx	:mu_LightningBlastArc.fx
		Lifespan 10
	End
	Event
		Type	Local
	
		At	root
		childfx	:mu_LightningBlastArc.fx
		Lifespan 10
	End
	Event
		Type	Local
	
		At	root
		childfx	:mu_LightningBlastArc.fx
		Lifespan 10
	End
end



Condition

	On 	cycle
	Time 	1
		Chance	.2
	Event
		Type	Local
	
		At	root
		childfx	:mu_LightningBlastArc.fx
		Lifespan 10
		
	End
end

Condition

	On 	time
	Time 	15
	Event
		
		Type	PositOnly
		At	Hips
		Sound mu_shockingbolt 100 100 .94
				
	End
End

Condition

	On 	time
	Time 	20

	Event
		Ename	Grounder
		Type	start  
		At	t_chest
#		bhvr	:mu_StaticArcTarget.bhvr
		#geom    Testing_Target
	end

	Event
		Ename	Arc1
		Type	start  
		At	chest
		bhvr	:mu_StaticArc_Start.bhvr
		###geom    Testing_Target
		magnet	Grounder
		LookAt	Grounder
	end

	Event
		Ename	Arc2
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_090_1.bhvr
		###geom    Testing_Target
	end
	
	Event
		Ename	Arc3
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_090_2.bhvr
		###geom    Testing_Target
	end
	
	Event
		Ename	Arc4
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_090_3.bhvr
		###geom    Testing_Target
	end
	
	Event
		Ename	Arc5
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_end.bhvr
		###geom    Testing_Target
	end

####################

	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		part1	:mu_StaticArcWalker2.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		part1	:mu_StaticArcWalker2.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		part1	:mu_StaticArcWalker2.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		part1	:mu_StaticArcWalker2.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end


End



Condition

	On 	time
	Time 	22


	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end


End



Condition

	On 	time
	Time 	24


	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end


End



Condition

	On 	time
	Time 	26


	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end


End


Condition

	On 	time
	Time 	28


	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	:mu_StaticArcWalker1.bhvr
		part1	:mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end


End

