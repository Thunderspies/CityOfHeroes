#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On 	Time
	Time 	9
	
	Event
		Type	Local
		At 	origin
		Sound Arachnosbot_blast1 100 100 .8
	End
End

Condition
	On 	Time
	Time 	39
	
	Event
		Type	Local
		At 	origin
		Sound robotlaserred2 100 100 1.0
	End
End


Condition
	On 	Time
	Time 	0 


	Event
		ENAME   cannon
		Type	local
		At	hair
		bhvr	:SmallBot_Blaster_off.bhvr
	End


	Event
		ename	genspot
		Type	local
		At	waist

		bhvr	:SmallBot_Blaster_Genspot.bhvr
	end

	Event
		ename	gens
		Type	local
		At	waist

		bhvr	:SmallBot_Blaster_Gens.bhvr
		part	:Smallbot_blaster_gensparksBig.part
	end

	Event
		Type	local
		At	cannon
		BhvrOverride
			PositionOffset		0 -.18 0
		End

		part	:Smallbot_blaster_chargering.part
		lifespan 36
	end


end

Condition
	On 	time
	Time 	38

	Event
		ENAME   prime
		Type	start
		At	cannon
		bhvr	:SmallBot_Blaster.bhvr
		Part1	:SmallBot_Blaster_BallTail.part
		magnet	t_chest
		lifespan 60
	End


	Event
		Type	local
		At	cannon

		Part1	:SmallBot_Blaster_ring.part
		Part	:SmallBot_Blaster_ring2.part
	end


	Event
		ENAME   screw
		Type	local
		At	prime

		bhvr	:SmallBot_Blaster_Twist.bhvr
	end

	Event
		Type	local
		At	screw

		bhvr	:SmallBot_Blaster_TwistOff.bhvr
		Part1	:SmallBot_Blaster_Twist2.part
		Part1	:SmallBot_Blaster_Twist2.part
		lifespan 60
	end


	Event
		Type	local
		At	screw

		bhvr	:SmallBot_Blaster_TwistOff2.bhvr
		Part1	:SmallBot_Blaster_Twist2.part
		Part1	:SmallBot_Blaster_Twist2.part
		lifespan 60
	end
	Event
		Type	local
		At	genspot
		part1	:SmallBot_Blaster_ringSmoke.part
		part1	:SmallBot_Blaster_ringSmokeN.part
		part1	:SmallBot_Blaster_ringSteam.part
		part1	:SmallBot_Blaster_ringSteamN.part
		pmagnet root
	end

End	
	

Condition
	On 	time
	Time 	42




	Event
		ENAME   prime1
		Type	start
		At	cannon
		bhvr	:SmallBot_Blaster.bhvr
		Part1	:SmallBot_Blaster_BallTail.part
#		Part1	:SmallBot_Blaster_BallGlow.part
#		Part1	:SmallBot_Blaster_BallStar.part
#		Part1	:SmallBot_Blaster_SteamTrail.part
		magnet	t_head
		lifespan 60
	End


	Event
		Type	local
		At	cannon

		Part1	:SmallBot_Blaster_ring.part
		Part	:SmallBot_Blaster_ring2.part
#		Part1	:SmallBot_Blaster_flameB.part
		pmagnet root
		lifespan 7
	end


	Event
		ENAME   screw1
		Type	local
		At	prime1

		bhvr	:SmallBot_Blaster_Twist.bhvr
	end

	Event
		Type	local
		At	screw1

		bhvr	:SmallBot_Blaster_TwistOff.bhvr
		Part1	:SmallBot_Blaster_Twist2.part
		Part1	:SmallBot_Blaster_Twist2.part
		lifespan 60
	end


	Event
		Type	local
		At	screw1

		bhvr	:SmallBot_Blaster_TwistOff2.bhvr
		Part1	:SmallBot_Blaster_Twist2.part
		Part1	:SmallBot_Blaster_Twist2.part
		lifespan 60
	end
	Event
		Type	local
		At	genspot
		part1	:SmallBot_Blaster_ringSmoke.part
		part1	:SmallBot_Blaster_ringSmokeN.part
		pmagnet root
	end


End		

Condition
	On 	time
	Time 	46




	Event
		ENAME   prime2
		Type	start
		At	cannon
		bhvr	:SmallBot_Blaster.bhvr
		Part1	:SmallBot_Blaster_BallTail.part
#		Part1	:SmallBot_Blaster_BallGlow.part
#		Part1	:SmallBot_Blaster_BallStar.part
#		Part1	:SmallBot_Blaster_SteamTrail.part
		magnet	t_waist
		lifespan 60
	End


	Event
		Type	local
		At	cannon

		Part	:SmallBot_Blaster_ring2.part
		Part1	:SmallBot_Blaster_ring.part
#		Part1	:SmallBot_Blaster_flameB.part
		pmagnet root
		lifespan 7
	end


	Event
		ENAME   screw2
		Type	local
		At	prime2

		bhvr	:SmallBot_Blaster_Twist.bhvr
	end

	Event
		Type	local
		At	screw2

		bhvr	:SmallBot_Blaster_TwistOff.bhvr
		Part1	:SmallBot_Blaster_Twist2.part
		Part1	:SmallBot_Blaster_Twist2.part
		lifespan 60
	end


	Event
		Type	local
		At	screw2

		bhvr	:SmallBot_Blaster_TwistOff2.bhvr
		Part1	:SmallBot_Blaster_Twist2.part
		Part1	:SmallBot_Blaster_Twist2.part
		lifespan 60
	end
	Event
		Type	local
		At	genspot
		part1	:SmallBot_Blaster_ringSmoke.part
		part1	:SmallBot_Blaster_ringSmokeN.part
		pmagnet root
	end


End	


Condition
	On 	primehit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End
end


Condition
	On 	prime1hit
	
	Event
		EName 	Prime1
		Type	Destroy
		

	End
end


Condition
	On 	prime2hit
	
	Event
		EName 	Prime2
		Type	Destroy
		

	End
end