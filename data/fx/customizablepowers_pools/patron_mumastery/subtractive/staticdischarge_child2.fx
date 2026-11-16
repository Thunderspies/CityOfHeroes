#########################################################
##	Predefined arc curve
########################################################
FxInfo

LifeSpan	60

########################################################



Condition

	On 	time
	Time 	0


	#################
	##Reds
	###################################
	Event
		Type	local
		At	wepL
		Part1	:mu_Cage_HandArchs2.part
		Part1	:mu_Cage_HandArchs_White1.part
		lifespan  5
	End


#	#################
#	##Whites
#	###################################
#	Event
#		Type	local
#		At	wepL
#		Part1	V_COV\EnemyPowers\Arachnos\mu_Cage_HandArchs_White2.part
#		lifespan  35
#	End

End


Condition

	On 	time
	Time 	15
	Chance	0.3
	Event

		Type	PositOnly
		At	Hips
		#Sound	elecballhit2 100 30 .2

	End
End

Condition

	On 	time
	Time 	20

	Event
		Ename	Grounder
		Type	start
		At	t_root
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcTarget.bhvr
		#geom    Testing_Target
	end

	Event
		Ename	Arc1
		Type	start
		At	origin ##chest
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_Start.bhvr
		###geom    Testing_Target
		magnet	Grounder
		LookAt	Grounder
	end

	Event
		Ename	Arc2
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_1.bhvr
		###geom    Testing_Target
	end

	Event
		Ename	Arc3
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_2.bhvr
		###geom    Testing_Target
	end

	Event
		Ename	Arc4
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_3.bhvr
		###geom    Testing_Target
	end

	Event
		Ename	Arc5
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_end.bhvr
		###geom    Testing_Target
	end



########
	Event
		Ename	walker1
		Type	PositOnly
		At	arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end

	Event
		Type	local
		At	walker1
		Part	:mu_StaticArcWalker_Invert1a.part
		part1	:mu_StaticArcWalker1a.part
	#		Part	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker_Invert1a.part
		part1	:mu_StaticArcWalker1a.part
		lifespan 15
		###geom    Testing_Target
		Pmagnet	Root
#		pother	arc1
	end

########
	Event
		Ename	walker2
		Type	PositOnly
		At	Arc2
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end

	Event
		Type	local
		At	walker2
		Part	:mu_StaticArcWalker_Invert1a.part
		part1	:mu_StaticArcWalker1a.part
		Part	:mu_StaticArcWalker_Invert1a.part
	#		part1	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1a.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc2
	end

########
	Event
		Ename	walker3
		Type	PositOnly
		At	Arc3
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end

	Event
		Type	local
		At	walker3
		Part	:mu_StaticArcWalker_Invert1a.part
		part1	:mu_StaticArcWalker1a.part
	#		Part	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker_Invert1a.part
		part1	:mu_StaticArcWalker1a.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc2
	end

########

	Event
		Ename	walker4
		Type	PositOnly
		At	Arc4
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end

	Event
		Type	local
		At	walker4
		Part	:mu_StaticArcWalker_Invert1a.part
		part1	:mu_StaticArcWalker1a.part
		Part	:mu_StaticArcWalker_Invert1a.part
	#		part1	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1a.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc2
	end


End
