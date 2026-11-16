#########################################################
##	
##

FxInfo
#LifeSpan  150

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
	Time 	0

#	Event
#		ename	CANNON
#		Type	Local 
#		At	C_Hind_LLegR
##		geom    Testing_Target		
#		BhvrOverride
#			PositionOffset		0 0 -1.5
#		End
#	End

	Event
		ename	HANDCANNON
		Type	Local 
		At	LarmL
#		geom    Testing_Target		
		BhvrOverride
			PositionOffset		-1.4 -.08 -.1
		End
	End

end



#Condition
#	On 	Time
#	Time 	0
#
#	Event	
#		ename	Charge ##projected energy
#		Type	local
#		At	cannon	
##		geom    Testing_Target
#		bhvroverride
#			PyrRotate		0 0 90 
#			PositionOffset		.07 .051 .1
#			Scale			.7 .7 1.2
#		end
#		part	:BS_Tail_glow.part
#	end
#
#	Event	
#		ename	Charge2	#barrel energy
#		Type	local
#		At	cannon	
#		bhvroverride
#			PyrRotate		90 0 0  
#			PositionOffset		0 .1 .1
#			Scale			.4 .8 1.5
#		end
#		part	:BS_Tail_glow.part
#	end
#
#
#
#end


Condition
	On 	Time
	Time 	0

	Event	
		ename	HCharge  ##projected energy
		Type	local
		At	handcannon	
		bhvroverride
			PositionOffset		-.2 .22 0
			Scale			1 1.4 .8
		end
		part	:BS_Tail_glow.part
	end

	Event	
		ename	HCharge2 	#barrel energy
		Type	local
		At	handcannon	
		bhvroverride
			PyrRotate		 0 0 90 
			PositionOffset		0 .1 .051
			Scale			1 .6 1
		end
		part	:BS_Tail_glow.part
	end




end

Condition
	On 	Time
	Time 	0
	#################### these particles fade to black (invis) over time
#	Event	
#		ename	Charge3
#		Type	local
#		At	cannon	
#		part	:BS_WepSmoke.part
#		part	:BS_WepSmoke.part
##		lifespan 600
#	end

	Event	
		ename	HCharge3
		Type	local
		At	handcannon
		bhvroverride
			PositionOffset		-.3 0 0
		end	
		part	:BS_WepSmoke.part
		part	:BS_WepSmoke.part

	end
end


Condition
	On 	triggerbits
	Triggerbits	death

	
	Event
		EName 	cannon
		Type	destroy
	
	End
	Event
		EName 	handcannon
		Type	destroy
	
	End

end


End				