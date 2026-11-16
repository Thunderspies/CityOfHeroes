#########################################################
##	Predefined arc curve
########################################################
FxInfo

LifeSpan	30

########################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Collector
		Type	local
		At	Root
		BhvrOverride
			PositionOffset 0 -1 0
		End
		
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Thingy
		Type	local
		At	Collector
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound cagebolt2 80 80 .8
		
		

	End


	Event
		EName 	ThingyA
		Type	local
		At	Collector
		Part1	:ElectricityOuterGlow.part
		Part3	\POWERS\ElectricityControl\ElectricityRing.part
		
	End

End

Condition
	On 	Time
	Time 	15
	Event
		EName 	Burst
		Type	local
		At	Collector
		Part4	\POWERS\ElectricityControl\ElectricitySparkBurst.part
		Part4	\POWERS\ElectricityControl\ElectricitySparkBurst.part
		Sound mu_shockingbolt 100 100 .94
		LifeSpan	20

	End

End

Condition

	On 	time
	Time 	20

	Event
		Ename	Grounder
		Type	start  
		At	target
#		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcTarget.bhvr
		#geom    Testing_Target
	end

	Event
		Ename	Arc1
		Type	start  
		At	Collector
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArc_Start.bhvr
		###geom    Testing_Target
		magnet	Grounder
		LookAt	Grounder
	end

	Event
		Ename	Arc2
		Type	local  
		At	Arc1
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_1.bhvr
		###geom    Testing_Target
	end
	
	Event
		Ename	Arc3
		Type	local  
		At	Arc1
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_2.bhvr
		###geom    Testing_Target
	end
	
	Event
		Ename	Arc4
		Type	local  
		At	Arc1
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_3.bhvr
		###geom    Testing_Target
	end
	
	Event
		Ename	Arc5
		Type	local  
		At	Arc1
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArc_end.bhvr
		###geom    Testing_Target
	end

####################

	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker2.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker2.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker2.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker2.part
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
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
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
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
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
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
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
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end
	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		part1	\V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.part
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end


End

