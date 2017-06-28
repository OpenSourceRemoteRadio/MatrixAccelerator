 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %  
 % Copyright (c) 2017, BigCat Wireless Pvt Ltd
 % All rights reserved.
 % 
 % Redistribution and use in source and binary forms, with or without
 % modification, are permitted provided that the following conditions are met:
 % 
 %     * Redistributions of source code must retain the above copyright notice,
 %       this list of conditions and the following disclaimer.
 %
 %     * Redistributions in binary form must reproduce the above copyright
 %       notice, this list of conditions and the following disclaimer in the
 %       documentation and/or other materials provided with the distribution.
 %
 %     * Neither the name of the copyright holder nor the names of its contributors
 %       may be used to endorse or promote products derived from this software
 %       without specific prior written permission.
 % 
 % 
 % 
 % THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 % AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 % IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 % DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 % FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 % DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 % SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 % CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 % OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 % OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 % 
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Accelerator_A10_param - DSPBA Design Parameters Start

%% System Parameters
Accelerator_A10_param.ChanCount                 = 0;                        % How many real data channels
Accelerator_A10_param.ClockRate                 = 491.520000;               % The system clock rate in MHz
Accelerator_A10_param.SampleRate                = 491.520000;               % The data rate per channel in MSps (mega-samples per second)
Accelerator_A10_param.ClockMargin               = 0.0;                      % Adjust the pipelining effort



%% Simulation Parameters
Accelerator_A10_param.SampleTime                = 1;            % One unit in Simulink simulation is one clock cycle 
Accelerator_A10_param.endTime                   = 15000;        % How many simulation clock cycles
Accelerator_A10_param.ContiguousValidFrames     = 1;            % Create a sequence of valid and invalid frames of stimulus data in the Channelizer block
Accelerator_A10_param.ContiguousInvalidFrames   = 0;            % This will produce all valid frames


%% Derived Parameters 
Accelerator_A10_param.Period                    = Accelerator_A10_param.ClockRate / Accelerator_A10_param.SampleRate;           % Clock cycles between consecutive data samples for a particular channel
Accelerator_A10_param.ChanWireCount             = ceil(Accelerator_A10_param.ChanCount/Accelerator_A10_param.Period);           % How many wires are needed to support the specified number of channels?
Accelerator_A10_param.ChanCycleCount            = ceil(Accelerator_A10_param.ChanCount/Accelerator_A10_param.ChanWireCount);    % Range of the channel signal
Accelerator_A10_param.ChanCycleCount            = ceil(Accelerator_A10_param.ChanCount/Accelerator_A10_param.ChanWireCount);    % Range of the channel signal
 % Loading random vectors 
 load('mat_acc_random_input_sets.mat');
 load('mat_acc_result_random_vector.mat');
 load('mat_acc_sop.mat');
 load('mat_acc_valid.mat');

%% DSPBA Design Parameters End
